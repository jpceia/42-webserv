/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 17:30:40 by jceia             #+#    #+#             */
/*   Updated: 2022/03/14 16:09:36 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "TCPServer.hpp"
# include "HTTPServer.hpp"
# include "HTTPRequest.hpp"
# include "HTTPResponse.hpp"
# include "HTTPConnection.hpp"
# include "HTTPListener.hpp"
# include "HTTPStatefulConnection.hpp"
# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>
# include <algorithm>
# include <sys/poll.h>
# include "utils.hpp"


HTTPServer::HTTPServer(configFile config, int timeout) :
    TCPServer(timeout),
    _config(config)
{
}

HTTPServer::~HTTPServer()
{
}


void HTTPServer::init()
{
    std::set<std::pair<std::string, int> > ips_and_ports;
    std::vector<configServerBlock> server_blocks = _config.getServerBlocksObj();
    for (std::vector<configServerBlock>::iterator it = server_blocks.begin();
        it != server_blocks.end(); ++it)
        ips_and_ports.insert(std::make_pair(it->getIP(), it->getPort()));
    for (std::set<std::pair<std::string, int> >::const_iterator it = ips_and_ports.begin();
        it != ips_and_ports.end(); ++it)
    {
        // filter for ip and port
        std::vector<configServerBlock> serverBlocks;
        for (std::vector<configServerBlock>::iterator it2 = server_blocks.begin();
            it2 != server_blocks.end(); ++it2)
            if (it->first == it2->getIP() && it->second == it2->getPort())
                serverBlocks.push_back(*it2);

        this->add_listener(new HTTPListener(it->first, it->second, serverBlocks));
    }
}

int HTTPServer::_handle_client_recv(TCPConnection* connection)
{
    HTTPStatefulConnection* conn = dynamic_cast<HTTPStatefulConnection*>(connection);
    if (conn == NULL)
        throw std::runtime_error("HTTPServer::_handle_client_request: dynamic_cast failed");

    bool finished = conn->recvChunk();
    if (finished) // Finished receiving the request
    {
        HTTPRequest request = conn->getRequest();

        // Get the server block
        configServerBlock server_block = conn->getServerBlock(request.getHeader("Host"));
        
        // Get correct location config for a given path
        configLocationBlock location_block = server_block.getLocationByPath(request.getPath());

        // Construct the context
        Context ctx;
        //ctx.error_page = location_block.getErrorPage();
        ctx.max_body_size = location_block.getClientMaxBodySize();
        ctx.root = location_block.getRoot();
        ctx.index = location_block.getIndex();
        {
            std::vector<std::string> methods = location_block.getMethods();
            for (std::vector<std::string>::iterator it = methods.begin();
                it != methods.end(); ++it)
                ctx.allowed_methods.push_back(HTTPRequest::parseMethod(*it));
        }
        ctx.redirect_status = location_block.getRedirectStatus();
        ctx.redirect_path = location_block.getRedirectPath();
        // cgi
        ctx.upload_path = location_block.getUpload();
        ctx.server_name = server_block.getServerName();
        ctx.server_addr = connection->getServerIP();
        ctx.client_addr = connection->getClientIP();
        ctx.server_port = connection->getServerPort();
        ctx.client_port = connection->getClientPort();

        // build the response
        HTTPResponse response = _response(conn->getRequest(), ctx);
        conn->setResponse(response);
        return POLLOUT;
    }
    return POLLIN;
}

int HTTPServer::_handle_client_send(TCPConnection* connection)
{
    HTTPStatefulConnection* conn = dynamic_cast<HTTPStatefulConnection*>(connection);
    if (conn == NULL)
        throw std::runtime_error("HTTPServer::_handle_client_request: dynamic_cast failed");

    bool finished = conn->sendChuck();
    if (finished)
    {
        if (conn->getRequest().getHeader("Connection") != "keep-alive") // close connection
            _close_connection(connection);
        conn->clear();  // Clear the buffers
        return POLLIN;
    }
    return POLLOUT;
}

HTTPResponse HTTPServer::_response(const HTTPRequest& request, Context& ctx)
{
    // checking if the method is allowed
    if (std::find(ctx.allowed_methods.begin(), ctx.allowed_methods.end(),
        request.getMethod()) == ctx.allowed_methods.end())
        return _method_not_allowed_response(ctx);

    // checking the body size
    if (request.getBody().size() > (size_t)ctx.max_body_size)
        return _body_too_large_response(ctx);

    // checking if the file exists
    ctx.path = ctx.root + request.getEndpoint();
    if (is_dir(ctx.path))
    {
        std::cout << "path is a directory" << std::endl;
        bool found = false;
        for (std::vector<std::string>::const_iterator it = ctx.index.begin();
            it != ctx.index.end(); ++it)
        {
            if (is_readable_file(ctx.path + *it))
            {
                ctx.path += *it;
                found = true;
                break;
            }
        }
        if (!found)
            return _not_found_response(ctx);
    }
    else if (!is_readable_file(ctx.path))
        return _not_found_response(ctx);

    // CGI
    if(ctx.path.substr(ctx.path.find_last_of(".") + 1) == "php")
        return _cgi_response(request, ctx);
    
    // Static page
    std::ifstream ifs(ctx.path.c_str(), std::ifstream::in);
    if (!ifs.good())
        return _not_found_response(ctx);

    std::cout << "path is " << ctx.path << std::endl;

    HTTPResponse response;
    response.setVersion("HTTP/1.1");
    response.setHeader("Content-Type", "text/html");
    response.setStatus(200, "OK");
    response.setBody(ifs);
    ifs.close();
    return response;
}

HTTPResponse HTTPServer::_cgi_response(const HTTPRequest& request, const Context& ctx)
{
    // calling the CGI script using execvpe
    std::vector<std::string> args;
    args.push_back("php");
    args.push_back(ctx.path);

    std::map<std::string, std::string> env;

    env["SERVER_NAME"] = ctx.server_name;
    env["SERVER_PROTOCOL"] = request.getVersion();
    env["SERVER_SOFTWARE"] = "webserv";
    env["AUTH_TYPE"] = "";
    env["CONTENT_LENGTH"] = request.getHeader("Content-Length");
    env["CONTENT_TYPE"] = request.getHeader("Content-Type");
    env["DOCUMENT_ROOT"] = ctx.root;
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["PATH_INFO"] = request.getPath();

    // HTTP info
    env["HTTP_ACCEPT"] = request.getHeader("Accept");
    env["HTTP_ACCEPT_ENCODING"] = request.getHeader("Accept-Encoding");
    env["HTTP_ACCEPT_LANGUAGE"] = request.getHeader("Accept-Language");
    env["HTTP_CONNECTION"] = request.getHeader("Connection");
    env["HTTP_HOST"] = ctx.server_name; // TODO: get the hostname from config
    env["HTTP_USER_AGENT"] = request.getHeader("User-Agent");
    env["HTTP_COOKIE"] = request.getHeader("Cookie");
    
    env["QUERY_STRING"] = request.getQueryString();
    env["REQUEST_METHOD"] = HTTPRequest::strMethod(request.getMethod());
    
    env["SERVER_ADDR"] = ctx.server_addr;
    env["REMOTE_ADDR"] = ctx.client_addr;

    std::stringstream ss;
    ss << ctx.server_port;
    ss >> env["SERVER_PORT"];
    ss.clear();
    ss << ctx.client_port;
    ss >> env["REMOTE_PORT"];

    std::string body = exec_cmd("php", args, env);

    std::cout << "cgi output: " << body << std::endl;

    HTTPResponse response;
    response.setVersion("HTTP/1.1");
    response.setHeader("Content-Type", "text/html");
    response.setStatus(200, "OK");
    response.setBody(body);
    return response;
}

HTTPResponse HTTPServer::_not_found_response(const Context& ctx) const
{
    HTTPResponse response;
    response.setVersion("HTTP/1.1");
    response.setHeader("Content-Type", "text/html");
    response.setStatus(404, "Not Found");
    
    std::string path = ctx.root + "/404.html";
    std::ifstream ifs(path.c_str(), std::ifstream::in);
    if (ifs.good())
    {
        response.setBody(ifs);
        ifs.close();
    }
    else
        response.setBody("<h1>404 Not Found</h1>"); // backup

    return response;
}

HTTPResponse HTTPServer::_method_not_allowed_response(const Context& ctx) const
{
    (void)ctx;
    HTTPResponse response;
    response.setVersion("HTTP/1.1");
    response.setHeader("Content-Type", "text/html");
    response.setStatus(405, "Method Not Allowed");
    response.setBody("<h1>405 Method Not Allowed</h1>");
    return response;
}

HTTPResponse HTTPServer::_body_too_large_response(const Context& ctx) const
{
    (void)ctx;
    HTTPResponse response;
    response.setVersion("HTTP/1.1");
    response.setHeader("Content-Type", "text/html");
    response.setStatus(413, "Request Entity Too Large");
    response.setBody("<h1>413 Request Entity Too Large</h1>");
    return response;
}

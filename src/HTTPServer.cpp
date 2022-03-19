/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 17:30:40 by jceia             #+#    #+#             */
/*   Updated: 2022/03/19 13:07:02 by jpceia           ###   ########.fr       */
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

        std::cout << "Request: " << std::endl;
        std::cout << request << std::endl;

        // Get the server block
        configServerBlock server_block = conn->getServerBlock(request.getHeader("Host"));
        
        // Get correct location config for a given path
        configLocationBlock location_block = server_block.getLocationByPath(request.getPath());

        // Construct the context
        Context ctx;
        ctx.error_page = location_block.getErrorPage();
        ctx.max_body_size = location_block.getClientMaxBodySize();
        ctx.root = location_block.getRoot();
        ctx.rel_path = request.getEndpoint().substr(location_block.getLocationPath().size());
        ctx.base_path = ctx.root + "/" + ctx.rel_path;
        ctx.index = location_block.getIndex();
        ctx.autoindex = location_block.getAutoIndex();
        {
            std::vector<std::string> methods = location_block.getMethods();
            for (std::vector<std::string>::iterator it = methods.begin();
                it != methods.end(); ++it)
                ctx.allowed_methods.push_back(HTTPRequest::parseMethod(*it));
        }
        ctx.redirect_status = location_block.getRedirectStatus();
        ctx.redirect_path = location_block.getRedirectPath();
        // cgi
        ctx.cgi = location_block.getCgi();
        ctx.upload_path = location_block.getUpload();
        if (server_block.getServerName().size() > 0)
            ctx.server_name = server_block.getServerName().front();
        ctx.server_addr = connection->getServerIP();
        ctx.client_addr = connection->getClientIP();
        ctx.server_port = connection->getServerPort();
        ctx.client_port = connection->getClientPort();
    
        // build the response
        HTTPResponse response = _response(conn->getRequest(), ctx);
        std::cout << "Response: " << std::endl;
        std::cout << response << std::endl;
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
        return _error_page_response(405, "Method not allowed", ctx);

    // checking the body size
    if (request.getBody().size() > (size_t)ctx.max_body_size)
        return _error_page_response(413, "Request Entity Too Large", ctx);

    // checking if it is a redirection
    if (!ctx.redirect_path.empty())
        return _redirect_response(ctx);

    if (!ctx.upload_path.empty())
        return _upload_response(request, ctx);

    // checking if the file exists
    ctx.path = ctx.base_path;
    if (is_dir(ctx.path))
    {
        std::cout << "path is a directory" << std::endl;
        bool found = false;
        for (std::vector<std::string>::const_iterator it = ctx.index.begin();
            it != ctx.index.end(); ++it)
        {
            std::cout << "path: " << ctx.base_path << "\t index:" << *it << std::endl;
            if (is_readable_file(ctx.path + "/" + *it))
            {
                ctx.path = ctx.base_path + "/" +  *it;
                found = true;
                break ;
            }
        }
        if (!found)
        {
            if (ctx.autoindex == "on")
                return _autoindex_response(ctx);
            else
                return _error_page_response(404, "Not found", ctx);
        }
    }
    else if (!is_readable_file(ctx.path))
        return _error_page_response(404, "Not found", ctx);

    // CGI
    std::string extension = ctx.path.substr(ctx.path.find_last_of(".") + 1);
    std::map<std::string, std::string>::const_iterator it = ctx.cgi.find(extension);
    if (it != ctx.cgi.end())
        return _cgi_response(it->second, request, ctx);

    // Static page
    std::ifstream ifs(ctx.path.c_str(), std::ifstream::in);
    if (!ifs.good())
    {
        // cannot open file
        return _error_page_response(500, "Internal Server Error", ctx);
    }

    std::cout << "path is " << ctx.path << std::endl;

    HTTPResponse response;
    response.setVersion("HTTP/1.1");
    response.setHeader("Content-Type", "text/html");
    response.setStatus(200, "OK");
    response.setBody(ifs);
    ifs.close();
    return response;
}

HTTPResponse HTTPServer::_cgi_response(const std::string& cmd, const HTTPRequest& request, const Context& ctx)
{
    // calling the CGI script using execvpe
    std::vector<std::string> args;
    args.push_back(cmd);
    //args.push_back(ctx.path);

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
    env["SCRIPT_NAME"] = ctx.path;
    env["SCRIPT_FILENAME"] = ctx.path;
    env["REDIRECT_STATUS"] = ctx.redirect_path;

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

    std::string body = exec_cmd(cmd, args, env);

    std::cout << "cgi output: " << body << std::endl;

    HTTPResponse response;
    response.setVersion("HTTP/1.1");
    response.setHeader("Content-Type", "text/html");
    response.setStatus(200, "OK");
    response.setBody(body);
    return response;
}

HTTPResponse HTTPServer::_autoindex_response(const Context& ctx) const
{
    /*
    std::stringstream ss;
    ss << "<html><head><title>Index of " << ctx.path << "</title></head><body><h1>Index of " << ctx.path << "</h1><hr><pre>";
    ss << "<a href=\"..\">..</a>\n";
    for (std::vector<std::string>::const_iterator it = ctx.index.begin();
        it != ctx.index.end(); ++it)
    {
        std::string path = ctx.path + *it;
        if (is_dir(path))
            ss << "<a href=\"" << *it << "/\">" << *it << "/</a>\n";
        else
            ss << "<a href=\"" << *it << "\">" << *it << "</a>\n";
    }
    ss << "</pre></body></html>";
    std::string body = ss.str();
    */
    (void)ctx;
    HTTPResponse response;
    response.setVersion("HTTP/1.1");
    response.setHeader("Content-Type", "text/html");
    response.setStatus(200, "OK");
    response.setBody("Autoindex");
    //response.setBody(body);
    return response;
    
}

HTTPResponse HTTPServer::_redirect_response(const Context& ctx) const
{
    HTTPResponse response;

    std::string text;
    response.setVersion("HTTP/1.1");
    response.setHeader("Location", ctx.redirect_path);
    switch (ctx.redirect_status)
    {
    case 300:
        text = "Multiple Choices";
        break ;
    case 301:
        text = "Moved Permanently";
        break ;
    case 302:
        text = "Found";
        break ;
    case 303:
        text = "See Other";
        break ;
    case 304:
        text = "Not Modified";
        break ;
    case 305:
        text = "Use Proxy";
        break ;
    case 307:
        text = "Temporary Redirect";
        break ;
    case 308:
        text = "Permanent Redirect";
        break ;
    default:
        break;
    }
    response.setStatus(ctx.redirect_status, text);
    return response;
}

HTTPResponse HTTPServer::_upload_response(const HTTPRequest& request, const Context& ctx) const
{
    std::cout << "Entering upload response" << std::endl;
    HTTPResponse response;
    response.setVersion("HTTP/1.1");
    response.setHeader("Content-Type", "text/html");
    HTTPMethod method = request.getMethod();
    if (method != POST && method != PUT)
    {
        response.setStatus(405, "Method Not Allowed");
        return response;
    }
    std::string path = ctx.upload_path + ctx.rel_path;
    std::cout << "Upload path: " << path << std::endl;
    // write to file
    std::ofstream ofs(path.c_str(), std::ios::binary | std::ios::trunc);
    if (!ofs.good())
    {
        // cannot open file
        response.setStatus(500, "Internal Server Error");
        return response;
    }
    std::string body = request.getBody();
    ofs.write(body.c_str(), body.size());
    ofs.close();

    response.setStatus(200, "OK");
    response.setBody("Upload Successfull");
    return response;
}

HTTPResponse HTTPServer::_error_page_response(int code, const std::string& msg, const Context& ctx) const
{
    HTTPResponse response;
    response.setVersion("HTTP/1.1");
    response.setHeader("Content-Type", "text/html");
    response.setStatus(code, msg);

    std::map<int, std::string>::const_iterator it = ctx.error_page.find(code);
    if (it != ctx.error_page.end())
    {
        std::ifstream ifs(it->second.c_str(), std::ifstream::in);
        if (ifs.good())
        {
            response.setBody(ifs);
            ifs.close();
            return response;
        }
    }

    // Backup
    std::stringstream ss;
    ss << "<h1>" << code << " " << msg << "</h1>";
    response.setBody(ss.str());
    return response;
}

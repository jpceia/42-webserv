/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 17:30:40 by jceia             #+#    #+#             */
/*   Updated: 2022/03/11 19:59:03 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "TCPServer.hpp"
# include "HTTPServer.hpp"
# include "HTTPRequest.hpp"
# include "HTTPResponse.hpp"
# include "HTTPConnection.hpp"
# include "HTTPStatefulConnection.hpp"
# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>
# include <algorithm>
# include <sys/poll.h>
# include "utils.hpp"


HTTPServer::HTTPServer(int timeout) :
    TCPServer(timeout),
    _root("./www"),
    _name(""),
    _max_body_size(1024 * 1024) // 1MB
{
    _index.push_back("index.html");
    _index.push_back("index.htm");

    _allowed_methods.push_back(GET);
    _allowed_methods.push_back(POST);
    _allowed_methods.push_back(DELETE);
    
    //this->init();
}

HTTPServer::~HTTPServer()
{
}

int HTTPServer::_handle_client_recv(TCPConnection* connection)
{
    HTTPStatefulConnection* conn = dynamic_cast<HTTPStatefulConnection*>(connection);
    if (conn == NULL)
        throw std::runtime_error("HTTPServer::_handle_client_request: dynamic_cast failed");

    bool finished = conn->recvChunk();
    if (finished) // Finished receiving the request
    {
        // Construct the context
        Context ctx;
        ctx.server_addr = connection->getServerIP();
        ctx.client_addr = connection->getClientIP();
        ctx.server_port = connection->getServerPort();
        ctx.client_port = connection->getClientPort();

        // build the response
        HTTPResponse response = _build_response(conn->getRequest(), ctx);
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

HTTPResponse HTTPServer::_build_response(const HTTPRequest& request, Context& ctx)
{
    // checking if the method is allowed
    if (std::find(_allowed_methods.begin(), _allowed_methods.end(),
        request.getMethod()) == _allowed_methods.end())
        return _method_not_allowed_response();

    // checking the body size
    if (request.getBody().size() > _max_body_size)
        return _body_too_large_response();

    // checking if the file exists
    ctx.path = _root + request.getEndpoint();
    if (is_dir(ctx.path))
    {
        std::cout << "path is a directory" << std::endl;
        bool found = false;
        for (std::vector<std::string>::const_iterator it = _index.begin();
            it != _index.end(); ++it)
        {
            if (is_readable_file(ctx.path + *it))
            {
                ctx.path += *it;
                found = true;
                break;
            }
        }
        if (!found)
            return _not_found_response();
    }
    else if (!is_readable_file(ctx.path))
        return _not_found_response();

    // CGI
    if(ctx.path.substr(ctx.path.find_last_of(".") + 1) == "php")
        return _build_cgi_response(request, ctx);
    
    // Static page
    std::ifstream ifs(ctx.path.c_str(), std::ifstream::in);
    if (!ifs.good())
        return _not_found_response();

    std::cout << "path is " << ctx.path << std::endl;

    HTTPResponse response;
    response.setVersion("HTTP/1.1");
    response.setHeader("Content-Type", "text/html");
    response.setStatus(200, "OK");
    response.setBody(ifs);
    ifs.close();
    return response;
}

HTTPResponse HTTPServer::_build_cgi_response(const HTTPRequest& request, const Context& ctx)
{
    // calling the CGI script using execvpe
    std::vector<std::string> args;
    args.push_back("php");
    args.push_back(ctx.path);

    std::map<std::string, std::string> env;

    env["SERVER_NAME"] = _name;
    env["SERVER_PROTOCOL"] = request.getVersion();
    env["SERVER_SOFTWARE"] = "webserv";
    env["AUTH_TYPE"] = "";
    env["CONTENT_LENGTH"] = request.getHeader("Content-Length");
    env["CONTENT_TYPE"] = request.getHeader("Content-Type");
    env["DOCUMENT_ROOT"] = _root;
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["PATH_INFO"] = request.getPath();

    // HTTP info
    env["HTTP_ACCEPT"] = request.getHeader("Accept");
    env["HTTP_ACCEPT_ENCODING"] = request.getHeader("Accept-Encoding");
    env["HTTP_ACCEPT_LANGUAGE"] = request.getHeader("Accept-Language");
    env["HTTP_CONNECTION"] = request.getHeader("Connection");
    env["HTTP_HOST"] = _name; // TODO: get the hostname from config
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

HTTPResponse HTTPServer::_not_found_response()
{
    HTTPResponse response;
    response.setVersion("HTTP/1.1");
    response.setHeader("Content-Type", "text/html");
    response.setStatus(404, "Not Found");
    
    std::string path = _root + "/404.html";
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

HTTPResponse HTTPServer::_method_not_allowed_response()
{
    HTTPResponse response;
    response.setVersion("HTTP/1.1");
    response.setHeader("Content-Type", "text/html");
    response.setStatus(405, "Method Not Allowed");
    response.setBody("<h1>405 Method Not Allowed</h1>");
    return response;
}

HTTPResponse HTTPServer::_body_too_large_response()
{
    HTTPResponse response;
    response.setVersion("HTTP/1.1");
    response.setHeader("Content-Type", "text/html");
    response.setStatus(413, "Request Entity Too Large");
    response.setBody("<h1>413 Request Entity Too Large</h1>");
    return response;
}

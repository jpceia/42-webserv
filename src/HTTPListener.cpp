/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPListener.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 17:30:40 by jceia             #+#    #+#             */
/*   Updated: 2022/03/04 10:57:44 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "HTTPListener.hpp"
# include "HTTPRequest.hpp"
# include "HTTPResponse.hpp"
# include "HTTPConnection.hpp"
# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>
# include <algorithm>
# include "utils.hpp"

HTTPListener::HTTPListener(const std::string& host, int port, int timeout) :
    TCPListener(host, port, timeout),
    _root("./www")
{
    _index.push_back("index.html");
    _index.push_back("index.htm");

    _allowed_methods.push_back(GET);
    _allowed_methods.push_back(POST);
    _allowed_methods.push_back(DELETE);
    
    this->init();
}

HTTPListener::~HTTPListener()
{
}

void HTTPListener::_handle_client_request(int fd)
{
    HTTPConnection connection(fd);

    // Read and parse the request
    HTTPRequest request = connection.recv();
    std::cout << request << std::endl;

    // Build the response
    connection.send(_build_response(request));
    if (request.getHeader("Connection") != "keep-alive") // close connection
        _close_fd(fd);
}

HTTPResponse HTTPListener::_build_response(const HTTPRequest& request)
{
    if (std::find(_allowed_methods.begin(), _allowed_methods.end(),
        request.getMethod()) == _allowed_methods.end())
        return _method_not_allowed_response();
    std::string path = _root + request.getPath();
    if (is_dir(path))
    {
        std::cout << "path is a directory" << std::endl;
        bool found = false;
        for (std::vector<std::string>::const_iterator it = _index.begin();
            it != _index.end(); ++it)
        {
            if (is_readable_file(path + *it))
            {
                path += *it;
                found = true;
                break;
            }
        }
        if (!found)
            return _not_found_response();
    }
    std::cout << "path is " << path << std::endl;

    HTTPResponse response;
    response.setVersion("HTTP/1.1");
    response.setHeader("Content-Type", "text/html");
    // get the file content using open
    std::ifstream ifs(path.c_str(), std::ifstream::in);
    if (ifs.good())
    {
        response.setStatus(200, "OK");
        response.setBody(ifs);
        ifs.close();
    }
    else
    {
        response.setStatus(404, "Not Found");
        response.setBody("<h1>404 Not Found</h1>");
    }
    return response;
}

HTTPResponse HTTPListener::_not_found_response()
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

HTTPResponse HTTPListener::_method_not_allowed_response()
{
    HTTPResponse response;
    response.setVersion("HTTP/1.1");
    response.setHeader("Content-Type", "text/html");
    response.setStatus(405, "Method Not Allowed");
    response.setBody("<h1>405 Method Not Allowed</h1>");
    return response;
}

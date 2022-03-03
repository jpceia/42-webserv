/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPListener.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jceia <jceia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 17:30:40 by jceia             #+#    #+#             */
/*   Updated: 2022/03/03 18:44:31 by jceia            ###   ########.fr       */
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

HTTPListener::HTTPListener(const std::string& host, int port, int timeout) :
    TCPListener(host, port, timeout),
    _root("./www")
{
    _index.push_back("index.html");
    _index.push_back("index.htm");
    this->init();
}

HTTPListener::~HTTPListener()
{
}

void HTTPListener::_handle_client_request(int fd)
{
    // handle message
    std::cout << "  Descriptor " << fd << " is readable" << std::endl;
    HTTPConnection connection(fd);

    // Read and parse the request
    HTTPRequest request = connection.recv();
    std::cout << "  Request: " << std::endl;
    std::cout << request << std::endl;

    // Build the response
    std::cout << "Building response" << std::endl;
    HTTPResponse response;
    response.setVersion("HTTP/1.1");
    response.setHeader("Content-Type", "text/html");

    std::string path = _root + request.getPath();

    // get the file content using open
    std::ifstream file(path.c_str(), std::ifstream::in);
    if (file.good())
    {
        response.setStatus(200, "OK");
        response.setBody(file);
        file.close();
    }
    else
    {
        std::cout << "File not found: " << path << std::endl;
        response.setStatus(404, "Not Found");
        path = _root + "/404.html";
        std::ifstream file_not_found(path.c_str(), std::ifstream::in);
        if (file_not_found)
        {
            response.setBody(file_not_found);
            file_not_found.close();
        }
        else
            response.setBody("<h1>404 Not Found</h1>"); // backup
    }

    // Send the response
    connection.send(response);
}


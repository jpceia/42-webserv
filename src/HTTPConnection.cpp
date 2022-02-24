/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPConnection.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 06:02:05 by jpceia            #+#    #+#             */
/*   Updated: 2022/02/24 05:40:53 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "HTTPConnection.hpp"
# include "TCPConnection.hpp"
# include "HTTPRequestParser.hpp"

HTTPConnection::HTTPConnection(int fd) :
    TCPConnection(fd)
{
}

HTTPConnection::HTTPConnection(const TCPConnection& rhs) :
    TCPConnection(rhs)
{
}

HTTPConnection::HTTPConnection(const HTTPConnection& rhs) :
    TCPConnection(rhs)
{
}

HTTPConnection::~HTTPConnection()
{
}

HTTPConnection& HTTPConnection::operator=(const HTTPConnection& rhs)
{
    TCPConnection::operator=(rhs);
    return *this;
}

void HTTPConnection::send(const HTTPResponse& response)
{
    std::stringstream ss;
    ss << response;
    TCPConnection::send(ss.str());
}

HTTPRequest HTTPConnection::recv()
{
    HTTPRequestParser request;
    ParseState state = PARSE_START;

    while (state != PARSE_COMPLETE)
        state = request.parse(TCPConnection::recv());
    return request;
}

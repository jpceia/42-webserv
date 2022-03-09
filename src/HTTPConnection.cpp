/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPConnection.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 06:02:05 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/09 20:20:01 by jpceia           ###   ########.fr       */
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

void HTTPConnection::sendResponse(const HTTPResponse& response) const
{
    std::stringstream ss;
    ss << response;
    std::string msg = ss.str();
    while (!msg.empty())
        TCPConnection::send(msg);
}

HTTPRequest HTTPConnection::recvRequest() const
{
    HTTPRequestParser request;
    ParseState state = PARSE_START;

    while (state != PARSE_COMPLETE)
    {
        std::string chunk = TCPConnection::recv();
        if (chunk.empty())
            throw TCPConnection::DisconnectedException();
        state = request.parse(chunk);
    }
    return request;
}

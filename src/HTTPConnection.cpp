/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPConnection.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 06:02:05 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/16 23:55:31 by jpceia           ###   ########.fr       */
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
    size_t size = BUFF_SIZE;

    while (state != PARSE_COMPLETE)
    {
        std::string chunk = TCPConnection::recv(size);
        state = request.parse(chunk);
        if (chunk.size() < size && request.isByChunks() && state != PARSE_START)
            state = PARSE_COMPLETE;
        else if (chunk.empty())
            throw TCPConnection::DisconnectedException();
    }
    return request;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPStatefulConnection.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 20:22:56 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/09 20:27:04 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"
#include "HTTPRequestParser.hpp"
#include "HTTPStatefulConnection.hpp"

HTTPStatefulConnection::HTTPStatefulConnection(int fd) :
    HTTPConnection(fd)
{
}

HTTPStatefulConnection::HTTPStatefulConnection(const TCPConnectionArgs& args, const std::vector<configServerBlock>& configs) :
    HTTPConnection(args),
    _configs(configs)
{
}

HTTPStatefulConnection::HTTPStatefulConnection(const HTTPConnection& rhs) :
    HTTPConnection(rhs),
    _configs(rhs._configs)
{
}

HTTPStatefulConnection::HTTPStatefulConnection(const HTTPStatefulConnection& rhs) :
    HTTPConnection(rhs),
    _request(rhs._request),
    _response(rhs._response)
{
}

HTTPStatefulConnection::~HTTPStatefulConnection()
{
}

HTTPStatefulConnection& HTTPStatefulConnection::operator=(const HTTPStatefulConnection& rhs)
{
    HTTPConnection::operator=(rhs);
    if (this != &rhs)
    {
        _request = rhs._request;
        _response = rhs._response;
    }
    return *this;
}

HTTPRequest HTTPStatefulConnection::getRequest() const
{
    if (_request.getState() != PARSE_COMPLETE)
        throw std::runtime_error("HTTPStatefulConnection::getRequest() - "
                                    "request not complete");
    return _request;
}

void HTTPStatefulConnection::setResponse(const HTTPResponse& response)
{
    std::stringstream ss;
    ss << response;
    _response = ss.str();
}

bool HTTPStatefulConnection::sendChuck()
{
    if (_response.empty())
        return true;
    TCPConnection::send(_response);
    return _response.empty();
}

bool HTTPStatefulConnection::recvChunk()
{
    ParseState state = _request.getState();
    if (state == PARSE_COMPLETE)
        throw std::runtime_error("HTTPStatefulConnection::recv() - "
                                    "request already complete");
    std::string chunk = TCPConnection::recv();
    if (chunk.empty())
        throw TCPConnection::DisconnectedException();
    return _request.parse(chunk) == PARSE_COMPLETE;
}

void HTTPStatefulConnection::clear()
{
    _request.clear();
    _response.clear();
}

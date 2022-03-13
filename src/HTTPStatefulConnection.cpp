/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPStatefulConnection.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 20:22:56 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/13 17:53:53 by jpceia           ###   ########.fr       */
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

configServerBlock HTTPStatefulConnection::getServerBlock(const std::string& host) const
{
    configServerBlock config = _configs.front();
    // iterate over the configs and find the one with the same host
    for (std::vector<configServerBlock>::const_iterator it = _configs.begin();
        it != _configs.end(); ++it)
    {
        std::vector<std::string> server_names = it->getServerName();
        std::vector<std::string>::const_iterator it_server_name = std::find(
            server_names.begin(), server_names.end(), host);
        if (it_server_name != server_names.end())
        {
            config = *it;
            break
        }
    }
    return config;
};

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPConnection.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 03:04:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/08 23:23:18 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "TCPConnection.hpp"
# include <map>

TCPConnection::TCPConnection(int sock) :
    _sock(sock)
{
    if (_sock < 0)
        throw std::runtime_error("Invalid file descriptor");
}

TCPConnection::TCPConnection(int sock,
    const struct sockaddr_in& server_addr,
    const struct sockaddr_in& client_addr) :
    _sock(sock),
    _server_addr(server_addr),
    _client_addr(client_addr)
{
    if (_sock < 0)
        throw std::runtime_error("Invalid file descriptor");
}

TCPConnection::TCPConnection(const TCPConnection& rhs) :
    _sock(rhs._sock),
    _server_addr(rhs._server_addr),
    _client_addr(rhs._client_addr)
{
}

TCPConnection::~TCPConnection()
{
}

TCPConnection& TCPConnection::operator=(const TCPConnection& rhs)
{
    if (this != &rhs)
    {
        _sock = rhs._sock;
        _server_addr = rhs._server_addr;
        _client_addr = rhs._client_addr;
    }
    return *this;
}

void TCPConnection::send(const std::string& msg) const
{
    size_t pos = 0;
    while (pos < msg.size())
    {
        int n = ::send(_sock, msg.c_str() + pos, msg.size() - pos, 0);
        if (n < 0)
            throw TCPConnection::SendException();
        pos += n;
    }
}

std::string TCPConnection::recv() const
{
    char buff[BUFF_SIZE];

    int n = ::recv(_sock, buff, sizeof(buff), 0);
    if (n < 0)
        throw TCPConnection::ReadException();
    return std::string(buff, n);
}

int TCPConnection::getSock() const
{
    return _sock;
}

std::string TCPConnection::getServerIP() const
{
    return inet_ntoa(_server_addr.sin_addr);
}

std::string TCPConnection::getClientIP() const
{
    return inet_ntoa(_client_addr.sin_addr);
}

int TCPConnection::getServerPort() const
{
    return ntohs(_server_addr.sin_port);
}

int TCPConnection::getClientPort() const
{
    return ntohs(_client_addr.sin_port);
}

const char* TCPConnection::ConnectionException::what() const throw()
{
    return "Connection error";
}

const char* TCPConnection::SendException::what() const throw()
{
    return "Error sending message";
}

const char* TCPConnection::ReadException::what() const throw()
{
    return "Could not read from socket";
}

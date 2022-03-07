/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPConnection.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jceia <jceia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 03:04:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/07 15:07:42 by jceia            ###   ########.fr       */
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
    const struct sockaddr_in& host_addr,
    const struct sockaddr_in& client_addr) :
    _sock(sock),
    _host_addr(host_addr),
    _client_addr(client_addr)
{
    if (_sock < 0)
        throw std::runtime_error("Invalid file descriptor");
}

TCPConnection::TCPConnection(const TCPConnection& rhs) :
    _sock(rhs._sock),
    _host_addr(rhs._host_addr),
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
        _host_addr = rhs._host_addr;
        _client_addr = rhs._client_addr;
    }
    return *this;
}

void TCPConnection::send(const std::string& msg)
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

std::string TCPConnection::recv()
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

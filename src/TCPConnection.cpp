/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPConnection.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 03:04:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/25 17:04:44 by jpceia           ###   ########.fr       */
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

TCPConnection::TCPConnection(const TCPConnectionArgs& args) :
    _sock(args.sock),
    _server_addr(args.server_addr),
    _client_addr(args.client_addr)
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

void TCPConnection::send(std::string& msg) const
{
    if (msg.empty())
        return ;
    int n = ::send(_sock, msg.c_str(), msg.size(), 0);
    if (n < 0)
        throw TCPConnection::SendException();
    if (n == 0)
        throw TCPConnection::DisconnectedException();
    msg = msg.substr(n);
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

const char* TCPConnection::DisconnectedException::what() const throw()
{
    return "Disconnected";
}

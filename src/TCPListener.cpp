/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPListener.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 02:51:42 by jpceia            #+#    #+#             */
/*   Updated: 2022/02/23 16:49:50 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TCPListener.hpp"


TCPListener::TCPListener(const std::string& host, int port)
{
    _addr.sin_family = AF_INET;
    //_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    _addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &_addr.sin_addr);
}

TCPListener::TCPListener(const TCPListener& rhs) :
    _addr(rhs._addr),
    _sock(rhs._sock)
{
}

TCPListener::~TCPListener()
{
    close(_sock);
}

TCPListener& TCPListener::operator=(const TCPListener& rhs)
{
    if (this != &rhs)
    {
        _addr = rhs._addr;
        _sock = rhs._sock;
    }
    return *this;
}

void TCPListener::init()
{
    _sock = socket(AF_INET, SOCK_STREAM, 0);
    if (_sock < 0)
        throw TCPListener::CreateException();
    if (bind(_sock, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
        throw TCPListener::BindException();
    if (listen(_sock, SOMAXCONN) < 0)
        throw TCPListener::ListenException();
}

TCPConnection TCPListener::accept()
{
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    int connection = ::accept(_sock, (struct sockaddr *)&cli_addr, &cli_len);
    if (connection < 0)
        throw TCPListener::AcceptException();
    return connection;
}

const char* TCPListener::CreateException::what(void) const throw()
{
    return "Could not create socket";
}

const char* TCPListener::BindException::what(void) const throw()
{
    return "Could not bind socket";
}

const char* TCPListener::ListenException::what(void) const throw()
{
    return "Could not listen on socket";
}

const char* TCPListener::AcceptException::what(void) const throw()
{
    return "Could not accept connection";
}

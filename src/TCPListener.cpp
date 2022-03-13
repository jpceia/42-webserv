/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPListener.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 02:51:42 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/09 02:20:47 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TCPListener.hpp"
#include "utils.hpp"
#include <cstring>
#include <sstream>

TCPListener::TCPListener(int sock) :
    _sock(sock)
{
}

TCPListener::TCPListener(const std::string& host, int port)
{
    _addr.sin_family = AF_INET;
    //_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    _addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &_addr.sin_addr);
    std::memset(_addr.sin_zero, '\0', sizeof(_addr.sin_zero));
    
    _sock = socket(AF_INET, SOCK_STREAM, 0);
    if (_sock < 0)
        throw TCPListener::CreateException();
    int on = 1; //char yes='1'; // Solaris people use this
    // lose the pesky "Address already in use" error message
    
    if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        throw TCPListener::CreateException();

    if (bind(_sock, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
        throw TCPListener::BindException();

    if (listen(_sock, SOMAXCONN) < 0)
        throw TCPListener::ListenException();
}

TCPListener::TCPListener(const TCPListener& rhs)
{
    *this = rhs;
}

TCPListener::~TCPListener()
{
    // close(_sock);
}

TCPListener& TCPListener::operator=(const TCPListener& rhs)
{
    if (this != &rhs)
    {
        _sock = rhs._sock;
        _addr = rhs._addr;
    }
    return *this;
}

TCPConnection* TCPListener::accept() const
{
    return new TCPConnection(_accept());
}

TCPConnectionArgs TCPListener::_accept() const
{
    TCPConnectionArgs args;
    socklen_t cli_len = sizeof(args.client_addr);
    args.sock = ::accept(_sock, (struct sockaddr *)&args.client_addr, &cli_len);
    if (args.sock < 0)
        throw TCPListener::AcceptException();
    return args;
}

int TCPListener::getSock() const
{
    return _sock;
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

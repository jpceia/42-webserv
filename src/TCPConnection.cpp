/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPConnection.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jceia <jceia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 03:04:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/03 16:18:15 by jceia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "TCPConnection.hpp"
# include <map>

TCPConnection::TCPConnection(int fd) :
    _fd(fd)
{
    if (_fd < 0)
        throw std::runtime_error("Invalid file descriptor");
}

TCPConnection::TCPConnection(const TCPConnection& rhs) :
    _fd(rhs._fd)
{
}

TCPConnection::~TCPConnection()
{
}

TCPConnection& TCPConnection::operator=(const TCPConnection& rhs)
{
    if (this != &rhs)
    {
        _fd = rhs._fd;
    }
    return *this;
}

void TCPConnection::send(const std::string& msg)
{
    size_t pos = 0;
    while (pos < msg.size())
    {
        int n = ::send(_fd, msg.c_str() + pos, msg.size() - pos, 0);
        if (n < 0)
            throw TCPConnection::SendException();
        pos += n;
    }
}

std::string TCPConnection::recv()
{
    char buff[BUFF_SIZE];

    int n = ::recv(_fd, buff, sizeof(buff), 0);
    if (n < 0)
        throw TCPConnection::ReadException();
    return std::string(buff, n);
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

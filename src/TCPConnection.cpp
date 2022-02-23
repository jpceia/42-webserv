/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPConnection.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 03:04:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/02/23 04:17:22 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "TCPConnection.hpp"

TCPConnection::TCPConnection(int fd) :
    _fd(fd)
{
}

TCPConnection::TCPConnection(const TCPConnection& rhs) :
    _fd(rhs._fd)
{
}

TCPConnection::~TCPConnection()
{
    close(_fd);
}

TCPConnection& TCPConnection::operator=(const TCPConnection& rhs)
{
    if (this != &rhs)
        _fd = rhs._fd;
    return *this;
}

void TCPConnection::send(const std::string& msg)
{
    size_t pos = 0;
    while (pos < msg.size())
    {
        int n = ::send(_fd, msg.c_str() + pos, msg.size() - pos, 0);
        if (n < 0)
            throw std::runtime_error("Error sending message");
        pos += n;
    }
}

std::string TCPConnection::recv()
{
    char buff[BUFF_SIZE];

    if (::recv(_fd, buff, sizeof(buff), 0) < 0)
    {
        std::cerr << "Could not read from socket" << std::endl;
        throw std::exception();
    }
    return std::string(buff);
}

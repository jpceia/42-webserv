/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPConnection.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 03:04:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/02/23 07:11:07 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "TCPConnection.hpp"
# include <map>

std::map<int, size_t> TCPConnection::_ref_count;

TCPConnection::TCPConnection(int fd) :
    _fd(fd)
{
    if (_fd < 0)
        throw std::runtime_error("Invalid file descriptor");
    _increase_ref_count(_fd);
}

TCPConnection::TCPConnection(const TCPConnection& rhs) :
    _fd(rhs._fd)
{
    _increase_ref_count(_fd);
}

TCPConnection::~TCPConnection()
{
    _decrease_ref_count(_fd);
}

TCPConnection& TCPConnection::operator=(const TCPConnection& rhs)
{
    if (this != &rhs)
    {
        _decrease_ref_count(_fd);
        _fd = rhs._fd;
        _increase_ref_count(_fd);
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

const char* TCPConnection::SendException::what(void) const throw()
{
    return "Error sending message";
}

const char* TCPConnection::ReadException::what(void) const throw()
{
    return "Could not read from socket";
}

void TCPConnection::_increase_ref_count(int fd)
{
    if (_ref_count.find(fd) == _ref_count.end())
        _ref_count[fd] = 0;
    ++TCPConnection::_ref_count[fd];
}

void TCPConnection::_decrease_ref_count(int fd)
{
    if (_ref_count.find(fd) == _ref_count.end() || _ref_count[fd] == 0)
        throw std::runtime_error("Invalid file descriptor");
    --_ref_count[fd];
    if (_ref_count[fd] == 0)
    {
        _ref_count.erase(fd);
        close(fd);
    }
}

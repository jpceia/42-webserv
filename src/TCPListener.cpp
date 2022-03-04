/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPListener.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 02:51:42 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/04 14:33:26 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TCPListener.hpp"
#include "utils.hpp"
#include <cstring>
#include <sstream>

TCPListener::TCPListener(const std::string& host, int port, int timeout) :
    _port(port)
{
    _addr.sin_family = AF_INET;
    //_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    _addr.sin_port = htons(_port);
    inet_pton(AF_INET, host.c_str(), &_addr.sin_addr);
    std::memset(_addr.sin_zero, '\0', sizeof _addr.sin_zero);
    _timeout = timeout;
}

TCPListener::TCPListener(const TCPListener& rhs)
{
    *this = rhs;
}

TCPListener::~TCPListener()
{
    for (std::vector<struct pollfd>::iterator it = _fds.begin();
        it != _fds.end(); ++it)
        close(it->fd);
}

TCPListener& TCPListener::operator=(const TCPListener& rhs)
{
    if (this != &rhs)
    {
        _addr = rhs._addr;
        _sock = rhs._sock;
        _fds = rhs._fds;
    }
    return *this;
}

void TCPListener::init()
{
    _sock = socket(AF_INET, SOCK_STREAM, 0);
    if (_sock < 0)
        throw TCPListener::CreateException();

    int on = 1; //char yes='1'; // Solaris people use this
    // lose the pesky "Address already in use" error message
    if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on) < 0)
        throw TCPListener::CreateException();

    if (bind(_sock, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
        throw TCPListener::BindException();

    if (listen(_sock, SOMAXCONN) < 0)
        throw TCPListener::ListenException();

    // Adding the listening socket to the pollfd vector
    _fds.push_back(create_pollfd(_sock, POLLIN));
}

TCPConnection TCPListener::accept()
{
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    int connection = ::accept(_sock, (struct sockaddr *)&cli_addr, &cli_len);
    if (connection < 0)
        throw TCPListener::AcceptException();
    _fds.push_back(create_pollfd(connection, POLLIN));
    return connection;
}

void TCPListener::run() 
{
    try
    {
        while (true)
        {
            std::cout << "Waiting on poll()..." << std::endl;
            //printPollFds();
            //std::cout << " -----------------------------------------" << std::endl;

            int poll_ret = poll(&_fds[0], _fds.size(), _timeout);
            if (poll_ret < 0)  // poll failed
                throw std::runtime_error("Pool exception"); //TCPListener::PollException();
            if (poll_ret == 0) // timeout failed
                throw std::runtime_error("Timeout exception"); //TCPListener::TimeoutException();

            try
            {
                std::vector<struct pollfd>::iterator it = _fds.begin();
                for (;it != _fds.end() && !it->revents; ++it) ; // find the first ready fd
                if (it != _fds.end())  // If there's no activity skip _fds;
                    _handle_revent(it->fd, it->revents);
            }
            catch (TCPListener::PollHungUpException& e)
            {
                std::cerr << e.what() << std::endl;
            }
        }   /* End of loop through pollable descriptors */
    }
    catch(const std::exception& e)
    {
        std::cerr << "Unexpected error: " << e.what() << '\n';
    }
}

/**
 * returns true if the loop should continue
 */
void TCPListener::_handle_revent(int fd, int revents)
{
    std::cout << "Handling revents for fd " << fd << std::endl;
    if(revents & POLLHUP)
    {
        std::cerr << "  Error: revents = " << revents << std::endl;
        _close_fd(fd);
        throw TCPListener::PollHungUpException();
    }
    if (fd == _sock) // Accept a new connection
        TCPConnection client_connection = this->accept();
    else  // Not the listener socket, but an accepted (connected) socket. _fds[ >0].
    {   
        try
        {
            _handle_client_request(fd);
        }
        catch (const TCPConnection::ConnectionException& e)
        {
            std::cerr << e.what() << std::endl;
            _close_fd(fd);
        }
    }
}

int TCPListener::getPort() const
{
    return _port;
}

void TCPListener::_close_fd(int fd)
{
    std::vector<struct pollfd>::iterator it = _fds.begin();
    while (it != _fds.end() && it->fd != fd)
        ++it;
    if (it == _fds.end())
    {
        std::stringstream ss;
        ss << "TCPListener::_close_fd: fd " << fd << " not found";
        throw std::runtime_error(ss.str());
    }
    close(it->fd);
    _fds.erase(it);
    // *it = _fds.back();
    //_fds.pop_back();
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

const char* TCPListener::PollHungUpException::what(void) const throw()
{
    return "Poll hung up";
}

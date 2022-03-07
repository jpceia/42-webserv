/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jceia <jceia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 02:51:42 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/07 15:14:25 by jceia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TCPServer.hpp"
#include "TCPListener.hpp"
#include "utils.hpp"
#include <cstring>
#include <sstream>

TCPServer::TCPServer(int timeout) :
    _timeout(timeout)
{
}

TCPServer::TCPServer(const TCPServer& rhs)
{
    *this = rhs;
}

TCPServer::~TCPServer()
{
    for (std::vector<struct pollfd>::iterator it = _fds.begin();
        it != _fds.end(); ++it)
        close(it->fd);
}

TCPServer& TCPServer::operator=(const TCPServer& rhs)
{
    if (this != &rhs)
    {
        _listeners = rhs._listeners;
        _connections = rhs._connections;
        _fds = rhs._fds;
        _timeout = rhs._timeout;
    }
    return *this;
}

void TCPServer::add_listener(const TCPListener& listener)
{
    _listeners.insert(listener); // check if already exists (?)
    
    // Adding the listening socket to the pollfd vector
    _fds.push_back(create_pollfd(listener.getSock(), POLLIN));
}

void TCPServer::run() 
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
                throw std::runtime_error("Pool exception"); //TCPServer::PollException();
            if (poll_ret == 0) // timeout failed
                throw std::runtime_error("Timeout exception"); //TCPServer::TimeoutException();

            try
            {
                std::vector<struct pollfd>::iterator it = _fds.begin();
                for (;it != _fds.end() && !it->revents; ++it) ; // find the first ready fd
                if (it != _fds.end())  // If there's no activity skip _fds;
                    _handle_revent(it->fd, it->revents);
            }
            catch (TCPServer::PollHungUpException& e)
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
void TCPServer::_handle_revent(int fd, int revents)
{
    std::cout << "Handling revents for fd " << fd << std::endl;
    if(revents & POLLHUP)
    {
        std::cerr << "  Error: revents = " << revents << std::endl;
        _close_fd(fd);
        throw TCPServer::PollHungUpException();
    }
    std::set<TCPListener>::iterator it = _listeners.find(TCPListener(fd));
    if (it != _listeners.end())
    {
        TCPConnection connection = it->accept();
        _fds.push_back(create_pollfd(connection.getSock(), POLLIN));
        _connections.insert(connection);
    }
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

void TCPServer::_close_fd(int fd)
{
    std::vector<struct pollfd>::iterator it = _fds.begin();
    while (it != _fds.end() && it->fd != fd)
        ++it;
    if (it == _fds.end())
    {
        std::stringstream ss;
        ss << "TCPServer::_close_fd: fd " << fd << " not found";
        throw std::runtime_error(ss.str());
    }
    close(it->fd);
    _fds.erase(it);
    // *it = _fds.back();
    //_fds.pop_back();
}

const char* TCPServer::PollHungUpException::what(void) const throw()
{
    return "Poll hung up";
}

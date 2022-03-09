/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 02:51:42 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/09 02:23:56 by jpceia           ###   ########.fr       */
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
    for (listeners_t::iterator it = _listeners.begin();
        it != _listeners.end(); ++it)
        delete *it;
    for (connections_t::iterator it = _connections.begin();
        it != _connections.end(); ++it)
        delete *it;
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

void TCPServer::add_listener(TCPListener* listener)
{
    _listeners.insert(listener); // check if already exists (?)
    
    // Adding the listening socket to the pollfd vector
    _fds.push_back(create_pollfd(listener->getSock(), POLLIN));
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

void TCPServer::_close_connection(TCPConnection* connection)
{
    _close_fd(connection->getSock());
    _connections.erase(connection);
    delete connection;
}

void TCPServer::_close_listener(TCPListener* listener)
{
    _close_fd(listener->getSock());
    _listeners.erase(listener);
    delete listener;
}

/**
 * returns true if the loop should continue
 */
void TCPServer::_handle_revent(int fd, int revents)
{
    std::cout << "Handling revents for fd " << fd << std::endl;
    if(revents & POLLHUP)
    {
        std::cerr << "Poll Hung Up" << std::endl;
        connections_t::const_iterator it = _find_connection(fd);
        if (it != _connections.end())
        {
            _close_connection(*it);
            return ;
        }
        listeners_t::const_iterator it2 = _find_listener(fd);
        if (it2 != _listeners.end())
        {
            _close_listener(*it2);
            return ;
        }
        std::cerr << "Connection not found" << std::endl;
        _close_fd(fd);
    }

    listeners_t::const_iterator it = _find_listener(fd);
    if (it != _listeners.end())
    {
        TCPConnection* connection = (*it)->accept();
        _fds.push_back(create_pollfd(connection->getSock(), POLLIN));
        _connections.insert(connection);
    }
    else  // Not the listener socket, but an accepted (connected) socket. _fds[ >0].
    {
        connections_t::const_iterator it = _find_connection(fd);
        if (it == _connections.end())
        {
            std::cerr << "Connection not found" << std::endl;
            _close_fd(fd); // remove fd from pollfd vector
            return ;
        }
        try
        {
            _handle_client_request(*it);
        }
        catch (const TCPConnection::ConnectionException& e)
        {
            std::cerr << e.what() << std::endl;
            _close_connection(*it);
        }
    }
}

TCPServer::connections_t::iterator TCPServer::_find_connection(int fd)
{
    TCPConnection tmp(fd);
    return _connections.find(&tmp);
}

TCPServer::connections_t::const_iterator TCPServer::_find_connection(int fd) const
{
    TCPConnection tmp(fd);
    return _connections.find(&tmp);
}

TCPServer::listeners_t::iterator TCPServer::_find_listener(int fd)
{
    TCPListener tmp(fd);
    return _listeners.find(&tmp);
}

TCPServer::listeners_t::const_iterator TCPServer::_find_listener(int fd) const
{
    TCPListener tmp(fd);
    return _listeners.find(&tmp);
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

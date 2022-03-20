/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 02:51:42 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/20 00:25:23 by jpceia           ###   ########.fr       */
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
            #ifdef DEBUG
            std::cout << "Waiting on poll()..." << std::endl;
            #endif
            int poll_ret = poll(&_fds[0], _fds.size(), _timeout);
            if (poll_ret < 0)
                throw std::runtime_error("Pool exception");
            if (poll_ret == 0)
                throw std::runtime_error("Timeout exception");

            std::vector<struct pollfd>::iterator it = _fds.begin();
            for (;it != _fds.end() && !it->revents; ++it) ; // find the first ready fd
            if (it != _fds.end())  // If there's no activity skip _fds;
                _handle_revent(it->fd, it->events, it->revents);
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

void TCPServer::_handle_revent(int fd, short &events, short revents)
{
    #ifdef DEBUG
    std::cout << "Handling revents for fd " << fd << std::endl;
    #endif

    listeners_t::const_iterator it = _find_listener(fd);
    if (it != _listeners.end()) // the fd is from a listener
        _handle_listener_revent(*it, revents);
    else  
    {
        connections_t::const_iterator it = _find_connection(fd);
        if (it == _connections.end())
        {
            std::cerr << "Connection not found" << std::endl;
            _close_fd(fd); // remove fd from pollfd vector
            return ;
        }
        // Not the listener socket, but an accepted (connected) socket. _fds[ >0].
        try
        {
            _handle_connection_revent(*it, events, revents);
        }
        catch (const TCPConnection::ConnectionException& e)
        {
            std::cerr << e.what() << std::endl;
            _close_connection(*it);
        }
    }
}

void TCPServer::_handle_listener_revent(TCPListener* listener, short revents)
{
    if (revents & POLLIN)
    {
        #ifdef DEBUG
        std::cout << "POLLIN" << std::endl;
        #endif
        TCPConnection* connection = listener->accept();
        _fds.push_back(create_pollfd(connection->getSock(), POLLIN));
        _connections.insert(connection);
    }
    else if (revents & POLLERR)
    {
        std::cerr << "POLLERR" << std::endl;
        _close_listener(listener);
    }
    else if (revents & POLLHUP)
    {
        std::cerr << "POLLHUP" << std::endl;
        _close_listener(listener);
    }
    else
    {
        std::cerr << "Unexpected revents for listener:" << revents << std::endl;
        _close_listener(listener);
    }
}

void TCPServer::_handle_connection_revent(TCPConnection* connection, short& events, short revents)
{
    if (revents & POLLIN)
    {
        #ifdef DEBUG
        std::cout << "POLLIN" << std::endl;
        #endif
        events = _handle_client_recv(connection);
    }
    else if (revents & POLLOUT)
    {
        #ifdef DEBUG
        std::cout << "POLLOUT" << std::endl;
        #endif
        events = _handle_client_send(connection);
    }
    else if (revents & POLLHUP)
    {
        std::cerr << "POLLHUP" << std::endl;
        _close_connection(connection);
    }
    else if (revents & POLLERR)
    {
        std::cerr << "POLLERR" << std::endl;
        _close_connection(connection);
    }
    else if (revents & POLLNVAL)
    {
        std::cerr << "POLLNVAL" << std::endl;
        _close_connection(connection);
    }
    else
    {
        std::cerr << "Unknown revents for connection: " << revents << std::endl;
        _close_connection(connection);
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

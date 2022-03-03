/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPListener.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jceia <jceia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 02:51:42 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/03 17:16:46 by jceia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TCPListener.hpp"
#include <errno.h>


TCPListener::TCPListener(const std::string& host, int port, int timeout)
{
    _addr.sin_family = AF_INET;
    //_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    _addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &_addr.sin_addr);
    memset(_addr.sin_zero, '\0', sizeof _addr.sin_zero);

    /****************************************/
    /* Initialize the pollfd structure to 0 */
    /****************************************/
    memset(_fds, 0, sizeof(_fds));
    _timeout = timeout;
}

TCPListener::TCPListener(const TCPListener& rhs)
{
    *this = rhs;
}

TCPListener::~TCPListener()
{
    for (int i = 0; i < _nfds; i++)
    {
        if(_fds[i].fd >= 0)
            close(_fds[i].fd);
    }
}

TCPListener& TCPListener::operator=(const TCPListener& rhs)
{
    if (this != &rhs)
    {
        _addr = rhs._addr;
        _sock = rhs._sock;
        for (size_t i=0; i < 1000; i++) 
        {
            _fds[i].fd = rhs._fds[i].fd;
            _fds[i].events = rhs._fds[i].events;
            _fds[i].revents = rhs._fds[i].revents;
        }
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

    /************************************************/
    /* Setting the _fds[0] for the listening socket */
    /************************************************/
    _fds[0].fd = _sock;
    _fds[0].events = POLLIN;
    _nfds = 1;   
}

TCPConnection TCPListener::accept()
{
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    int connection = ::accept(_sock, (struct sockaddr *)&cli_addr, &cli_len);
    if (connection < 0)
    {
        if (errno == EWOULDBLOCK)
            throw TCPListener::WouldBlockException();
        throw TCPListener::AcceptException();
    }
    _fds[_nfds].fd = connection;
    _fds[_nfds].events = POLLIN;
    _fds[_nfds].revents = 0;
    ++_nfds;
    return connection;
}

void TCPListener::run() 
{
    try
    {
        while (true)
        {
            std::cout << "Waiting on poll()..." << std::endl;
            printPollFds();
            std::cout << " -----------------------------------------" << std::endl;

            int poll_ret = poll(_fds, _nfds, _timeout);
            if (poll_ret < 0)  // poll failed
                throw std::runtime_error("Pool exception"); //TCPListener::PollException();
            if (poll_ret == 0) // timeout failed
                throw std::runtime_error("Timeout exception"); //TCPListener::TimeoutException();

            try
            {
                for (int i = 0; i < _nfds; i++)
                    _poll_loop(i);  // call the poll loop for each fd
            }
            catch (TCPListener::PollHungUpException& e)
            {
                std::cout << e.what() << std::endl;
            }
            catch (TCPListener::WouldBlockException& e)
            {
                std::cout << e.what() << std::endl;
            }
        }    /* End of loop through pollable descriptors */
    }
    catch(const std::exception& e)
    {
        std::cerr << "Unexpected error: " << e.what() << '\n';
    }
}

/**
 * returns true if the loop should continue
 */
void TCPListener::_poll_loop(int i)
{
    int fd = _fds[i].fd;
    int revents = _fds[i].revents;
    
    if(revents == 0) // If there's no activity skip _fds;
        return ;

    if(revents & POLLHUP)
    {
        std::cerr << "  Error: revents = " << revents << std::endl;
        _close_fd(i);
        throw TCPListener::PollHungUpException();
    }

    // If the _fds[i].fd is the listener [0]
    if (fd == _sock)
    {
        std::cout << "  Listening socket is readable." << std::endl;
        TCPConnection client_connection = this->accept();
        client_connection.send("Connection accepted"); // check exceptions    
    }
    
    else  // Not the listener socket, but an accepted (connected) socket. _fds[ >0].
    {
        try
        {
            std::cout << "  Descriptor " << fd << " is readable" << std::endl;
            TCPConnection connection(fd);
            std::string msg = connection.recv();
            std::cout << msg.size() << " bytes received = " << std::endl;
            std::cout << msg << std::endl;
            if (msg.empty())
            {
                // connection closed by client
                _close_fd(i);
                return ;
            }
            connection.send(msg);
        }
        catch (const TCPConnection::ConnectionException& e)
        {
            std::cerr << e.what() << '\n';
            _close_fd(i);
        }
    }
}

void TCPListener::_close_fd(int index)
{
    close(_fds[index].fd);
    --_nfds;
    _fds[index] = _fds[_nfds];
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

const char* TCPListener::WouldBlockException::what(void) const throw()
{
    return "Would block Exception";
}

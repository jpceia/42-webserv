/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPListener.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jceia <jceia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 02:48:15 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/03 18:13:16 by jceia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPLISTENER_HPP
# define TCPLISTENER_HPP

# include "TCPConnection.hpp"
# include <iostream>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <stdio.h>
#include <sys/poll.h>

class TCPListener
{
public:
    TCPListener(const std::string& host, int port, int timeout);
    virtual ~TCPListener();

    void init();
    TCPConnection accept();
    void run();

    class CreateException : public std::exception
    {
        public:
            virtual const char* what(void) const throw();
    };

    class BindException : public std::exception
    {
        public:
            virtual const char* what(void) const throw();
    };

    class ListenException : public std::exception
    {
        public:
            virtual const char* what(void) const throw();
    };

    class AcceptException : public std::exception
    {
        public:
            virtual const char* what(void) const throw();
    };

    class PollHungUpException : public std::exception
    {
        public:
            virtual const char* what(void) const throw();
    };

    class WouldBlockException : public std::exception
    {
        public:
            virtual const char* what(void) const throw();
    };

protected:
    void _close_fd(int fd);
    virtual void _handle_client_request(int fd) = 0;

private:

    void	printPollFds()
    {
        for (int i = 0; i < _nfds; i++)
        {
            std::cout << "fds[" << i << "]" << std::endl;
            std::cout << "fd = " << _fds[i].fd << std::endl;
            std::cout << "events = " << _fds[i].events << std::endl;
            std::cout << "revents = " << _fds[i].revents << std::endl;

            std::cout << std::endl;
        }
    }

    void _poll_loop(int i);

    // Not copiable
    TCPListener(const TCPListener& rhs);
    TCPListener& operator=(const TCPListener& rhs);
    
    // Private attributes
    struct sockaddr_in _addr;
    int _sock;                      // listener sock


    /********************/
    /* poll() Variables */
    /********************/
    struct pollfd					_fds[1000];			// An array of pollfd structures.
                                                        // Containing the Listener at _fds[0]
                                                        // and clients at > 0
    int								_nfds;				// Number of connected clients
    int								_timeout;			// Maximum time, in milliseconds,
};

#endif

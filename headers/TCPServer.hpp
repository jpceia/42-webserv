/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 14:23:33 by jceia             #+#    #+#             */
/*   Updated: 2022/03/08 23:27:30 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPSERVER_HPP
# define TCPSERVER_HPP

# include "TCPConnection.hpp"
# include "TCPListener.hpp"
# include <iostream>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/poll.h>
# include <vector>
# include <set>

class TCPServer
{
public:
    TCPServer(int timeout);
    virtual ~TCPServer();

    void add_listener(const TCPListener& listener);
    void run();

    class PollHungUpException : public std::exception
    {
        public:
            virtual const char* what(void) const throw();
    };

protected:
    virtual void _handle_client_request(const TCPConnection& connection) = 0;
    virtual void _close_connection(const TCPConnection& connection);
    virtual void _close_listener(const TCPListener& listener);
        
    // Private attributes
    std::set<TCPListener, TCPListener::socket_compare>      _listeners;
    std::set<TCPConnection, TCPConnection::socket_compare>  _connections;

private:

    void	printPollFds()
    {
        for (size_t i = 0; i < _fds.size(); ++i)
        {
            std::cout << "fds[" << i << "]" << std::endl;
            std::cout << "fd = " << _fds[i].fd << std::endl;
            std::cout << "events = " << _fds[i].events << std::endl;
            std::cout << "revents = " << _fds[i].revents << std::endl;

            std::cout << std::endl;
        }
    }

    void _handle_revent(int fd, int revevents);
    void _close_fd(int fd);

    // Not copiable
    TCPServer(const TCPServer& rhs);
    TCPServer& operator=(const TCPServer& rhs);

    /* poll() Variables */
    std::vector<struct pollfd>  _fds;       // An array of pollfd structures.
                                            // Containing the Listener at _fds[0]
                                            // and clients at > 0
    int						    _timeout;	// Maximum time, in milliseconds,
};

#endif

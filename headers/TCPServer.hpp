/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 14:23:33 by jceia             #+#    #+#             */
/*   Updated: 2022/03/09 21:08:49 by jpceia           ###   ########.fr       */
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
    typedef std::set<TCPConnection*, TCPConnection::socket_compare> connections_t;
    typedef std::set<TCPListener*, TCPListener::socket_compare>     listeners_t;

    TCPServer(int timeout);
    virtual ~TCPServer();

    void add_listener(TCPListener* listener);
    void run();

protected:
    virtual int _handle_client_send(TCPConnection* connection) = 0; // return event to set in pollfd
    virtual int _handle_client_recv(TCPConnection* connection) = 0; // return event to set in pollfd
    virtual void _close_connection(TCPConnection* connection);
    virtual void _close_listener(TCPListener* listener);
    connections_t::iterator _find_connection(int fd);
    connections_t::const_iterator _find_connection(int fd) const;
    listeners_t::iterator _find_listener(int fd);
    listeners_t::const_iterator _find_listener(int fd) const;

    // Private attributes
    listeners_t     _listeners;
    connections_t   _connections;

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

    void _handle_revent(int fd, short &events, short revevents);
    void _handle_listener_revent(TCPListener* listener, short revents);
    void _handle_connection_revent(TCPConnection* connection, short &events, short revents);
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

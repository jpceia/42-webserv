/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 14:23:33 by jceia             #+#    #+#             */
/*   Updated: 2022/03/22 19:06:54 by jpceia           ###   ########.fr       */
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
    virtual void _handle_client_send(TCPConnection* connection, short& event) = 0;
    virtual void _handle_client_recv(TCPConnection* connection, short& event) = 0;
    virtual void _close_connection(TCPConnection* connection);
    virtual void _close_listener(TCPListener* listener);

private:
    connections_t::iterator _find_connection(int fd);
    connections_t::const_iterator _find_connection(int fd) const;
    listeners_t::iterator _find_listener(int fd);
    listeners_t::const_iterator _find_listener(int fd) const;

    void _handle_revent(int fd, short &events, short revevents);
    void _handle_listener_revent(TCPListener* listener, short revents);
    void _handle_connection_revent(TCPConnection* connection, short &events, short revents);
    void _close_fd(int fd);

    // Not copiable
    TCPServer(const TCPServer& rhs);
    TCPServer& operator=(const TCPServer& rhs);

    // Private attributes
    listeners_t     _listeners;
    connections_t   _connections;

    /* poll() Variables */
    std::vector<struct pollfd>  _fds;       // An array of pollfd structures.
                                            // Containing the Listener at _fds[0]
                                            // and clients at > 0
    int						    _timeout;	// Maximum time, in milliseconds,
};

#endif

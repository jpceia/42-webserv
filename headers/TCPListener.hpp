/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPListener.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 02:48:15 by jpceia            #+#    #+#             */
/*   Updated: 2022/02/23 03:15:32 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPLISTENER_HPP
# define TCPLISTENER_HPP

# include "TCPConnection.hpp"
# include <iostream>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <unistd.h>

class TCPListener
{
public:
    TCPListener(const std::string& host, int port);
    ~TCPListener();

    void init();
    TCPConnection accept();

private:
    // Not copiable
    TCPListener(const TCPListener& rhs);
    TCPListener& operator=(const TCPListener& rhs);
    
    // Private attributes
    struct sockaddr_in _addr;
    int _sock;
};

#endif

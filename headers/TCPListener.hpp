/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPListener.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 02:48:15 by jpceia            #+#    #+#             */
/*   Updated: 2022/02/23 03:00:07 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPLISTENER_HPP
# define TCPLISTENER_HPP

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
    int accept();

private:
    // Not copiable
    TCPListener(const TCPListener& rhs);
    TCPListener& operator=(const TCPListener& rhs);
    
    // Private attributes
    struct sockaddr_in _addr;
    int _sock;
};

#endif

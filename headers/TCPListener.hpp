/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPListener.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 02:48:15 by jpceia            #+#    #+#             */
/*   Updated: 2022/02/23 04:39:08 by jpceia           ###   ########.fr       */
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

private:
    // Not copiable
    TCPListener(const TCPListener& rhs);
    TCPListener& operator=(const TCPListener& rhs);
    
    // Private attributes
    struct sockaddr_in _addr;
    int _sock;
};

#endif

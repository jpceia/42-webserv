/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPConnection.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 03:04:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/02/23 07:18:31 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPCONNECTION_HPP
# define TCPCONNECTION_HPP

# include <iostream>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <sys/resource.h>
# include <map>

# define BUFF_SIZE 2048

class TCPConnection
{
public:
    TCPConnection(int fd);
    TCPConnection(const TCPConnection& rhs);

    virtual ~TCPConnection();
    TCPConnection& operator=(const TCPConnection& rhs);

    void send(const std::string& msg);
    std::string recv();

    int getFd() { return _fd; }

    class SendException : public std::exception
    {
        public:
            virtual const char* what(void) const throw();
    };
    
    class ReadException : public std::exception
    {
        public:
            virtual const char* what(void) const throw();
    };

private:
    int _fd;
};

#endif

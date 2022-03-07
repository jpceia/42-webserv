/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPConnection.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jceia <jceia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 03:04:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/07 16:06:52 by jceia            ###   ########.fr       */
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
    class socket_compare
    {
    public:
        bool operator()(const TCPConnection& lhs, const TCPConnection& rhs) const
        {
            return lhs._sock < rhs._sock;
        }
    private:
        friend class TCPConnection;
    };

    TCPConnection(int sock);
    TCPConnection(int sock, const struct sockaddr_in& server_addr, const struct sockaddr_in& client_addr);
    TCPConnection(const TCPConnection& rhs);

    virtual ~TCPConnection();
    TCPConnection& operator=(const TCPConnection& rhs);

    void send(const std::string& msg);
    std::string recv();

    int getSock() const;

    std::string getServerIP() const;
    std::string getClientIP() const;
    int getServerPort() const;
    int getClientPort() const;

    class ConnectionException : public std::exception
    {
        public:
            virtual const char* what(void) const throw();
    };

    class SendException : public TCPConnection::ConnectionException
    {
        public:
            virtual const char* what(void) const throw();
    };
    
    class ReadException : public TCPConnection::ConnectionException
    {
        public:
            virtual const char* what(void) const throw();
    };

private:
    int _sock;
    struct sockaddr_in _server_addr;
    struct sockaddr_in _client_addr;
};

#endif

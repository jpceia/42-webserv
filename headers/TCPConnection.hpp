/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPConnection.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 03:04:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/09 02:15:12 by jpceia           ###   ########.fr       */
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

struct TCPConnectionArgs
{
    int sock;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
};

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
    TCPConnection(const TCPConnectionArgs& args);
    TCPConnection(const TCPConnection& rhs);

    virtual ~TCPConnection();
    virtual TCPConnection& operator=(const TCPConnection& rhs);

    virtual void send(const std::string& msg) const;
    virtual std::string recv() const;

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

    class EmptyMessageException : public TCPConnection::ConnectionException
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

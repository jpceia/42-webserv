
#ifndef TCPLISTENER_HPP
# define TCPLISTENER_HPP

# include "TCPConnection.hpp"
# include <iostream>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/poll.h>
# include <vector>

class TCPListener
{
public:
    class socket_compare
    {
    public:
        bool operator()(const TCPListener& lhs, const TCPListener& rhs) const
        {
            return lhs._sock < rhs._sock;
        }
    private:
        friend class TCPListener;
    };

    TCPListener(int sock);
    TCPListener(const std::string& host, int port);
    TCPListener(const TCPListener& rhs);
    TCPListener& operator=(const TCPListener& rhs);
    virtual ~TCPListener();

    TCPConnection accept() const;

    int getSock() const;

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

    int _sock;
    struct sockaddr_in _addr;
};

#endif

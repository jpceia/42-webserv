#pragma once

#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sstream>
#include <unistd.h>

class TcpListener {
    public:

        TcpListener(const char* ipAddress, int port) : _ipAddress(ipAddress),
                                                       _port(port)
        {
            _addressLength = sizeof(_address);

            // We fill the sockaddr_in struct
            _address.sin_family = AF_INET;
            inet_pton(AF_INET, _ipAddress, &_address.sin_addr);
            _address.sin_port = htons(port);
            memset(_address.sin_zero, '\0', sizeof _address.sin_zero);    
        };

        ~TcpListener() 
        {};

        // Initialize the listener
        void init() {
            // Creating socket file descriptor
            if ((_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                perror("In socket");
                exit(EXIT_FAILURE);
            }

            // Bind the IP Address and Port to a Socket 
            if (bind(_socket_fd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
            {
                perror("In bind");
                exit(EXIT_FAILURE);
            }

            // listen() marks the socket referred to by _socket_fd as a passive
            // socket, that is, as a socket that will be used to accept incoming
            // connection requests using accept(2).
            // Second argument is the max pending connections 
            // SOMAXCONN = Max number of connections allowed to pass on this system
            if (listen(_socket_fd, SOMAXCONN) < 0)
            {
                perror("In listen");
                exit(EXIT_FAILURE);
            }
        };

        // Run the listener
        void run() {

        };


    private:
        const char                     *_ipAddress;      // IP Address server will run on
        int                             _port;           // Port # for the web service
        int                             _socket_fd;       // Internal FD for the listening socket 
        struct sockaddr_in              _address;        // Address struct
        int                             _addressLength;  // Length of the _address

};
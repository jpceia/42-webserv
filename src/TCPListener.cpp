/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPListener.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 02:51:42 by jpceia            #+#    #+#             */
/*   Updated: 2022/02/23 16:49:50 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TCPListener.hpp"


TCPListener::TCPListener(const std::string& host, int port, int timeout)
{
    _addr.sin_family = AF_INET;
    //_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    _addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &_addr.sin_addr);
    memset(_addr.sin_zero, '\0', sizeof _addr.sin_zero);

    /****************************************/
    /* Initialize the pollfd structure to 0 */
    /****************************************/
    memset(_fds, 0, sizeof(_fds));
    _timeout = timeout;
}

TCPListener::TCPListener(const TCPListener& rhs)
{
    *this = rhs;
}

TCPListener::~TCPListener()
{
    for (int i = 0; i < _nfds; i++)
    {
        if(_fds[i].fd >= 0)
            close(_fds[i].fd);
    }
}

TCPListener& TCPListener::operator=(const TCPListener& rhs)
{
    if (this != &rhs)
    {
        _addr = rhs._addr;
        _sock = rhs._sock;
        for (size_t i=0; i < 1000; i++) 
        {
            _fds[i].fd = rhs._fds[i].fd;
            _fds[i].events = rhs._fds[i].events;
            _fds[i].revents = rhs._fds[i].revents;
        }
    }
    return *this;
}

void TCPListener::init()
{
    _sock = socket(AF_INET, SOCK_STREAM, 0);
    if (_sock < 0)
        throw TCPListener::CreateException();

    int on = 1; //char yes='1'; // Solaris people use this
    // lose the pesky "Address already in use" error message
    if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on) < 0)
        throw TCPListener::CreateException();

    if (bind(_sock, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
        throw TCPListener::BindException();

    if (listen(_sock, SOMAXCONN) < 0)
        throw TCPListener::ListenException();
    
    /************************************************/
    /* Setting the _fds[0] for the listening socket */
    /************************************************/
    _fds[0].fd = _sock;
    _fds[0].events = POLLIN;
    _nfds = 1;   
}

TCPConnection TCPListener::accept()
{
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    int connection = ::accept(_sock, (struct sockaddr *)&cli_addr, &cli_len);
    if (connection < 0)
        throw TCPListener::AcceptException();
    _fds[_nfds].fd = connection;
    _fds[_nfds].events = POLLIN;
    _nfds++;
    return connection;
}

void TCPListener::run() 
{
    bool running 				= true;

    while (running == true)
    {
        printf("Waiting on poll()...\n");

		int poll_ret = poll(_fds, _nfds, _timeout);
        if (poll_ret < 0)  // poll failed
        {
            perror("  poll() failed");
            break;
        }
        if (poll_ret == 0) // timeout failed
        {
            printf("  poll() timed out.  End program.\n");
            break;
        }

        bool remove_connection = false;

        int		current_size = _nfds;
        for (int i = 0; i < current_size; i++)
        {
            if(_fds[i].revents == 0) // If there's no activity skip _fds;
                continue;

            if(_fds[i].revents != POLLIN)
            {
                printPollFds();
                printf("  Error! revents = %d\n", _fds[i].revents);
                running = false;
                break;
            }

            if (_fds[i].fd == _sock) // If the _fds[i].fd is the listener [0]
            {
                printf("  Listening socket is readable\n");

                TCPConnection new_client_socket = 0;

                //while (new_client_socket != -1)
                //{
                try
                {
                    new_client_socket = this->accept();
                }
                catch(const std::exception& e)
                {
                    std::cerr << "accept error" << '\n';
                    running = false;
                }

                new_client_socket.send("Connection accepted");
                printPollFds();
                //}
            }
            else  // Not the listener socket, but an accepted (connected) socket. _fds[ >0].
            {
                printPollFds();

                printf("  Descriptor %d is readable\n", _fds[i].fd);
                bool		close_client_connection = false;

                while(1)
                {

                    /*****************************************************/
                    /* Receive data on this connection until the         */
                    /* recv fails with EWOULDBLOCK. If any other         */
                    /* failure occurs, we will close the                 */
                    /* connection.                                       */
                    /*****************************************************/

                    TCPConnection connection(_fds[i].fd);
                    std::string msg;
                    try
                    {
                        msg = connection.recv();
                        if (msg.empty())
                        {
                            printf("  Connection closed\n");
                            close_client_connection = true;
                            break;
                        }
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << "recv failed" << '\n';
                        close_client_connection = true;
                        break ;
                    }

                    std::cout << msg.size() << "  %d bytes received = " << std::endl;
                    std::cout << msg << std::endl;

                    /*****************************************************/
                    /* Echo the data back to the client                  */
                    /*****************************************************/

                    //onMessageReceived(_fds[i].fd, buffer, bytes_recv);
                    //close_client_connection = true;

                    try
                    {
                        connection.send(msg);
                    }
                    catch (const std::exception& e)
                    {
                        std::cout << " send failed" << std::endl;
                        close_client_connection = true;
                        break;
                    }
                    break;
                }

                /*******************************************************/
                /* If the close_client_connection flag was turned on,  */
                /* we need to clean up this active connection. This    */
                /* clean up process includes removing the              */
                /* descriptor.                                         */
                /*******************************************************/
                if (close_client_connection == true)
                {
                    //onClientDisconnected(_fds[i].fd);
                    close(_fds[i].fd);
                    _fds[i].fd = -1;
                    remove_connection = true;
                }

            }  /* End of existing connection is readable */

        } /* End of loop through pollable descriptors */

        /***********************************************************/
        /* If the _remove_connection flag was turned on, we need   */
        /* to squeeze together the array and decrement the number  */
        /* of file descriptors.                                    */
        /***********************************************************/
        if (remove_connection)
        {
            remove_connection = false;
            for (int i = 0; i < _nfds; i++)
            {
                if (_fds[i].fd == -1)
                {
                    for(int j = i; j < _nfds; j++)
                    {
                        _fds[j].fd = _fds[j+1].fd;
                        _fds[j].events = _fds[j+1].events;
                        _fds[j].revents = _fds[j+1].revents;
                    }
                i--;
                _nfds--;
                }
            }
        }
    }
}

const char* TCPListener::CreateException::what(void) const throw()
{
    return "Could not create socket";
}

const char* TCPListener::BindException::what(void) const throw()
{
    return "Could not bind socket";
}

const char* TCPListener::ListenException::what(void) const throw()
{
    return "Could not listen on socket";
}

const char* TCPListener::AcceptException::what(void) const throw()
{
    return "Could not accept connection";
}

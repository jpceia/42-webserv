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
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <errno.h>

class TcpListener
{
    public:

        TcpListener(const char* ipAddress, int port) : _ipAddress(ipAddress),
                                                       _port(port)
        {};

        ~TcpListener()
        {};

		/***************************/
        /* Initialize the listener */
		/***************************/
        int		init(int timeout)
		{
			/***********************************/
            /* Creating socket file descriptor */
			/***********************************/
            if ((_listening_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			{
                perror("In socket");
                exit(EXIT_FAILURE);
            }

			/**********************************/
            /* We fill the sockaddr_in struct */
			/**********************************/
            _address.sin_family = AF_INET;
            inet_pton(AF_INET, _ipAddress, &_address.sin_addr);
            _address.sin_port = htons(_port);
            memset(_address.sin_zero, '\0', sizeof _address.sin_zero);

			/*********************************************************************/
            /* Bind the IP Address and Port to a Socket							 */
			/* When a socket is created with socket(2), it exists in a name		 */
       		/* space (address family) but has no address assigned to it.  bind() */
       		/* assigns the address specified by addr to the socket referred to	 */
       		/* by the file descriptor sockfd.									 */
			/*********************************************************************/
            if (bind(_listening_socket_fd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
			{
                perror("In bind");
                exit(EXIT_FAILURE);
            }

			/*******************************************************************************/
            /* listen() marks the socket referred to by _listening_socket_fd as a passive  */
            /* socket, that is, as a socket that will be used to accept incoming		   */
            /* connection requests using accept(2).										   */
            /* Second argument is the max pending connections 							   */
            /* SOMAXCONN = Max number of connections allowed to pass on this system        */
			/*******************************************************************************/
            if (listen(_listening_socket_fd, SOMAXCONN) < 0)
			{
                perror("In listen");
                exit(EXIT_FAILURE);
            }

			/****************/
			/* poll() stuff */
			/****************/

			/****************************************/
  			/* Initialize the pollfd structure to 0 */
			/****************************************/
  			memset(_fds, 0, sizeof(_fds));

			/************************************************/
			/* Setting the _fds[0] for the listening socket */
			/************************************************/
			_fds[0].fd = _listening_socket_fd;
			_fds[0].events = POLLIN;

			/*************************************/
			/* Initialize the timeout for poll() */
			/*************************************/
			_timeout = timeout;

			/************************************************************/
			/* Currently we only have the listener on the number of fds */
			/************************************************************/
			_nfds = 1;

			return (0);
        };

		/********************/
        /* Run the listener */
		/********************/
        void	run()
		{
			bool		running 				= true;

			while (running == true)
			{
				printf("Waiting on poll()...\n");

				int poll_ret = poll(_fds, _nfds, _timeout);

				/********************/
				/* if poll() failed */
				/********************/
				if (poll_ret < 0)
				{
					perror("  poll() failed");
					break;
				}

				/*****************************/
				/* if poll() timeout expired */
				/*****************************/
				if (poll_ret == 0)
				{
					printf("  poll() timed out.  End program.\n");
					break;
				}

				/**************************************************/
				/* One or more descriptors are readable.  Need to */
				/* determine which ones they are.                 */
				/**************************************************/
				int		current_size = _nfds;

				for (int i = 0; i < current_size; i++)
				{
					/******************************************************/
					/* Loop through to find the descriptors that returned */
					/* POLLIN and determine whether it's the listening    */
					/* or the active connection.                          */
					/******************************************************/

					/*************************************/
					/* If the _fds[i] returned an event. */
					/*************************************/
					if(_fds[i].revents == 0)
						continue;

					/**************************/
					/* If client used ctrl+c. */
					/**************************/
					if(_fds[i].revents & POLLHUP) 
					{
						printf("  Client ctrl+c / Hang UP! revents = %d\n", _fds[i].revents);						
						
						printPollFds();						
						remove_connection(i);
						break ;
					}
					/********************************************************/
					/* If revents is not POLLIN, it's an unexpected result, */
					/* log and end the server.								*/
					/* POLLIN is data to read.								*/
					/********************************************************/
					else if(_fds[i].revents != POLLIN)
					{
						printPollFds();
						printf("  Error! revents = %d\n", _fds[i].revents);
						running = false;
						break;
					}

					/***********************************************/
					/* If it's the listener socket (server socket) */
					/***********************************************/
					if (_fds[i].fd == _listening_socket_fd)
					{
						/*******************************************************/
						/* Listening descriptor is readable.                   */
						/*******************************************************/
						printf("  Listening socket is readable\n");

						/*****************************/
						/* Create new clients socket */
						/*****************************/
						int new_client_socket = 0;

						/*****************************************************/
						/* Accept each incoming connection. If               */
						/* accept fails with EWOULDBLOCK, then we            */
						/* have accepted all of them. Any other              */
						/* failure on accept will cause us to end the        */
						/* server.                                           */
						/*****************************************************/
						new_client_socket = accept(_listening_socket_fd, NULL, NULL);
						if (new_client_socket < 0)
						{
							if (errno != EWOULDBLOCK)
							{
								perror("  accept() failed");
								running = false;
							}
							break;
						}

						/*****************************************************/
						/* Add the new incoming connection to the            */
						/* pollfd structure                                  */
						/*****************************************************/
						printf("  New incoming connection - %d\n", new_client_socket);

						onClientConnected(new_client_socket);

						_fds[_nfds].fd = new_client_socket;
						_fds[_nfds].events = POLLIN;
						_nfds++;

						printPollFds();
					
					}
					/*********************************************************/
      				/* This is not the listening socket, therefore an        */
      				/* existing connection must be readable                  */
      				/*********************************************************/
					else
					{
						printPollFds();

						printf("  Descriptor %d is readable\n", _fds[i].fd);

						/*****************************************************/
						/* Receive data on this connection until the         */
						/* recv fails with EWOULDBLOCK. If any other         */
						/* failure occurs, we will close the                 */
						/* connection.                                       */
						/*****************************************************/
						char buffer[4096];
						int bytes_recv = recv(_fds[i].fd, buffer, sizeof(buffer), 0);
						if (bytes_recv < 0)
						{
							if (errno != EWOULDBLOCK)
							{
								perror("  recv() failed");
								remove_connection(i);
							}
							
							break ;
						}

						/*****************************************************/
						/* Check to see if the connection has been           */
						/* closed by the client                              */
						/*****************************************************/
						if (bytes_recv == 0)
						{
							printf("  Connection closed\n");
							remove_connection(i);
							
							break ;
						}

						/*****************************************************/
						/* Data was received                                 */
						/*****************************************************/
						printf("  %d bytes received = ", bytes_recv);
						printf("%s \n", buffer);

						/*****************************************************/
						/* Echo the data back to the client                  */
						/*****************************************************/

						int send_ret = onMessageReceived(_fds[i].fd, buffer, bytes_recv);
						if (send_ret < 0)
						{
							perror("  send() failed");

							remove_connection(i);							
							break ;
						}

						// Temporary to send msg back to client.
						// This is for chat, to receive it back.
						/*int send_ret = sendToClient(_fds[i].fd, buffer, bytes_recv);
						if (send_ret < 0)
						{
							perror("  send() failed");

							remove_connection(i);							
							break ;
						}*/


					}  /* End of existing connection is readable */

				} /* End of loop through pollable descriptors */
			}

			close_all_open_sockets();
		};

	protected:

		/*******************************************/
		/* Handler for client connections.		   */
		/* Virtual because it will be overwritten  */
		/* by multiple clients objects.			   */
		/*******************************************/
		virtual void	onClientConnected(int clientSocket)
		{};

		/*******************************************/
		/* Handler for client disconnections.	   */
		/* Virtual because it will be overwritten  */
		/* by multiple clients objects.			   */
		/*******************************************/
		virtual void	onClientDisconnected(int clientSocket)
		{};

		/**************************************************************/
		/* Handler for when the message is received from the client.  */
		/* Virtual because it will be overwritten                     */
		/* by multiple clients objects.			                      */
		/**************************************************************/
		virtual int	onMessageReceived(int clientSocket, const char*msg, int length)
		{ return (0); };

		/************************************/
		/* Send our message to our client.  */
		/************************************/
		int		sendToClient(int clientSocket, const char* msg, int length)
		{
			int send_ret = send(clientSocket, msg, length, 0);
			return (send_ret);
		};

		/****************************************/
		/* Broadcast a message for all clients  */
		/****************************************/
		void	broadcastToClients(int sendingClient, const char* msg, int length)
		{
			for (int i = 0; i < _nfds; i++)
			{
				/*****************************************************************/
				/* You don't want to send a message to the listener.			 */
				/* You also don't want to send a msg to the client that's 		 */
				/* sending messages.											 */
				/*****************************************************************/
				if (_fds[i].fd == _listening_socket_fd ||
					_fds[i].fd == sendingClient)
				{
					continue ;
				}

				if(_fds[i].revents == POLLIN && _fds[i].fd != sendingClient)
				{
					sendToClient(_fds[i].fd, msg, length);
				}
			}
		};

    private:

		/********************/
		/* Helper Functions */
		/********************/
		
		/**********************************************/
		/* Print all poll struct to check for changes */
		/**********************************************/
		void	printPollFds()
		{
			for (int i = 0; i < _nfds; i++)
			{
				std::cout << "fds[" << i << "]" << std::endl;
				std::cout << "fd = " << _fds[i].fd << std::endl;
				std::cout << "events = " << _fds[i].events << std::endl;
				std::cout << "revents = " << _fds[i].revents << std::endl;

				std::cout << std::endl;
			}
		}

		/***********************************************************/
		/* If remove connection function was called, we need to    */
		/* squeeze together the array and decrement the number     */
		/* of file descriptors.                                    */
		/***********************************************************/
		void remove_connection(int i) 
		{
			close(_fds[i].fd);
			_fds[i].fd = -1;

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
		};

		/*************************************************************/
		/* Clean up all of the sockets that are open                 */
		/*************************************************************/
		void	close_all_open_sockets()
		{
			for (int i = 0; i < _nfds; i++)
			{
				if(_fds[i].fd >= 0)
				close(_fds[i].fd);
			}
		}

		/******************/
		/* Data Variables */
		/******************/

        const char                      *_ipAddress;     		// IP Address server will run on
        int                             _port;           		// Port # for the web service
        int                             _listening_socket_fd;   // Internal FD for the listening socket
        struct sockaddr_in              _address;       		// Address struct

		/********************/
		/* poll() Variables */
		/********************/
        struct pollfd					_fds[1000];			// An array of pollfd structures.
															// Containing the Listener at _fds[0]
															// and clients at > 0
		int								_nfds;				// Number of connected clients
		int								_timeout;			// Maximum time, in milliseconds,
															// to wait for the poll function
															// to complete.
};

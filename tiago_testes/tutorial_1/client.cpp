#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

/* 
    Programming with TCP/IP sockets

    There are a few steps involved in using sockets:

        1- Create the socket               

            A socket, is created with the socket system call            
            int socket_fd = socket(domain, type, protocol);

        2- Identify the socket

            When we talk about naming a socket, we are talking about assigning a transport
            address to the socket (a port number in IP networking). In sockets, this operation
            is called binding an address and the bind system call is used for this.

            int bind(int socket, const struct sockaddr *address, socklen_t address_len);

            struct sockaddr_in { 
                __uint8_t         sin_len; 
                sa_family_t       sin_family; 
                in_port_t         sin_port; 
                struct in_addr    sin_addr; 
                char              sin_zero[8]; 
            };

        3- On the server, wait for an incoming connection

            Before a client can connect to a server, the server should have a socket that is prepared
            to accept the connections. The listen system call tells a socket that it should be capable
            of accepting incoming connections.
                
        4- Send and receive messages

            The same read and write system calls that work on files also work on sockets.
           
            int valread = read( new_socket , buffer, 1024); 
            printf(“%s\n”,buffer );
            if(valread < 0)
            { 
                printf("No bytes are there to read");
            }

        5- Close the socket

            Close a socket with the close system call — the same close that is used for files.
*/

#define PORT 8080

int main(int argc, char const *argv[])
{
    int sock = 0; long valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}
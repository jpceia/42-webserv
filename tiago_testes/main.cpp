#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

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
            of accepting incoming connections
        
        
        4- Send and receive messages
        5- Close the socket
*/

int main(int argc, char **argv) {
    
    // Socket FD
    int server_fd;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0) < 0)) {
        perror("Cannot create socket");
        return (0);
    }

    // Filling socketaddr
    struct sockaddr_in address;

    const int PORT = 8080;  // Where clients can reach at.
    
    /* htonl converts a long integer (e.g. address) to a network representation */
    /* htons converts a short integer (e.g. port) to a network representation */ 
    
    memset((char *)&address, 0, sizeof(address)); 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = htonl(INADDR_ANY); 
    address.sin_port = htons(PORT); if (bind(server_fd,(struct sockaddr *)&address,sizeof(address)) < 0) { 
        perror("bind failed"); 
        return 0; 
    }

}

#include "webserv.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{
    int port = 8000;
    
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Could not create socket" << std::endl;
        return -1;
    }
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "Could not bind socket" << std::endl;
        return -1;
    }

    listen(sockfd, 256); // not sure about the last parameter

    socklen_t cli_len = sizeof(cli_addr);
    int connection = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);
    if (connection < 0)
    {
        std::cerr << "Could not accept connection" << std::endl;
        return -1;
    }
    // read from the socket
    char buffer[2048];
    int n = read(connection, buffer, sizeof(buffer)); // we must use pool instead of read 
    if (n < 0)
    {
        std::cerr << "Could not read from socket" << std::endl;
        return -1;
    }
    std::cout << buffer << std::endl;
    // write to the socket
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello World</h1></body></html>";
    n = write(connection, response.c_str(), response.length()); // we must use write instead of read 
    if (n < 0)
    {
        std::cerr << "Could not write to socket" << std::endl;
        return -1;
    }
    // close the connection
    close(connection);

    // close the socket
    close(sockfd);
    return 0;
}

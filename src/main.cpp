#include "webserv.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "TCPListener.hpp"
#include "TCPConnection.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFF_SIZE 1024

// https://gist.github.com/vthanki/8405c9cd4a09d3a0b73bf876b2635ad4#file-unix_server-c

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: ./webserv <port>" << std::endl;
        return -1;
    }
    int port;
    std::stringstream ss(argv[1]);
    ss >> port;

    TCPListener listener("0.0.0.0", port);
    listener.init();
    TCPConnection connection = listener.accept();
    // parse the request
    HTTPRequest request;
    ss.str("");
    ss.clear();
    std::string recv_msg = connection.recv();
    ss << recv_msg;
    ss >> request;
    std::cout << request << std::endl;
    // write to the socket
    HTTPResponse response;
    response.setVersion("HTTP/1.1");
    response.setStatus(200, "OK");
    response.setHeader("Content-Type", "text/html");
    response.setBody("<html><body><h1>Hello World</h1></body></html>");
    ss.str("");
    ss.clear();
    ss << response;
    std::string send_msg = ss.str();
    connection.send(send_msg);
    return 0;
}

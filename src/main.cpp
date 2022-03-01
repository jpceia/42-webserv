#include "webserv.hpp"
#include "utils.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "TCPListener.hpp"
#include "HTTPConnection.hpp"
#include <iostream>
#include <sstream>

// https://gist.github.com/vthanki/8405c9cd4a09d3a0b73bf876b2635ad4#file-unix_server-c

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: ./webserv <port>" << std::endl;
        return -1;
    }

    try
    {
        TCPListener listener("0.0.0.0", ft_stoi(argv[1])); // populates the listener addresss
        listener.init();                                   // creates a socket + bind + listen


        // Receiving connections
        HTTPConnection connection = listener.accept();

        // Read and parse the request
        HTTPRequest request = connection.recv();
        std::cout << request << std::endl;

        // Build the response
        HTTPResponse response;
        response.setVersion("HTTP/1.1");
        response.setStatus(200, "OK");
        response.setHeader("Content-Type", "text/html");
        response.setBody("<html><body><h1>Hello World</h1></body></html>");

        // Send the response
        connection.send(response);
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }

    return 0;
}

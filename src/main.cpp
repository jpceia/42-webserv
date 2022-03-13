#include "webserv.hpp"
#include "utils.hpp"
#include "HTTPServer.hpp"
#include "HTTPListener.hpp"
#include "HTTPConnection.hpp"
#include <iostream>
#include <sstream>

// https://gist.github.com/vthanki/8405c9cd4a09d3a0b73bf876b2635ad4#file-unix_server-c

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: ./webserv <port1> <port2> ..." << std::endl;
        return -1;
    }

    try
    {
        HTTPServer webserv(-1);
        for (int i=1; i<argc; i++)
            webserv.add_listener(new HTTPListener("0.0.0.0", ft_stoi(argv[i])));
        webserv.run();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}

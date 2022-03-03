#include "webserv.hpp"
#include "utils.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "HTTPListener.hpp"
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
        HTTPListener listener("0.0.0.0", ft_stoi(argv[1]), -1); // populates the listener addresss
        listener.run();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}

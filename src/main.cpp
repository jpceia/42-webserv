#include "webserv.hpp"
#include "utils.hpp"
#include "HTTPServer.hpp"
#include "HTTPListener.hpp"
#include "HTTPConnection.hpp"
#include "configFile.hpp"
#include <iostream>
#include <sstream>

// https://gist.github.com/vthanki/8405c9cd4a09d3a0b73bf876b2635ad4#file-unix_server-c

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Insert the Configuration File" << std::endl;
        return (1);
    }

    try
    {
        configFile  config(argv[1]);

        HTTPServer webserv(config, -1);
        webserv.init();
        webserv.run();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}

#include "utils.hpp"
#include "HTTPServer.hpp"
#include "configFile.hpp"
#include <iostream>

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

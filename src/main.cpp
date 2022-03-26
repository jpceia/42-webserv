#include "utils.hpp"
#include "HTTPServer.hpp"
#include "configFile.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    std::string path = "main.conf";

    if (argc == 2)
        path = argv[1];
    else if (argc > 2)
    {
        std::cout << "Insert the configuration file" << std::endl;
        return 1;
    }
    try
    {
        configFile  config(path);

        HTTPServer webserv(config, -1);
        webserv.init();
        webserv.run();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}

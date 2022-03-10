#include <iostream>
#include "./include/configFile.hpp"

int main(int argc, char **argv)
{
    std::string line;
    if (argc != 2)
    {
        std::cout << "Insert the Configuration File" << std::endl;
        return (1);
    }

	try
	{
		configFile  config_obj(argv[1]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

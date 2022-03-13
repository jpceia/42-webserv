#include <iostream>
#include "./include/configFile.hpp"

#define DEBUG 1

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

		//config_obj.getServerBlocksObj().begin()->printDirectives();

		if (DEBUG == 1)
		{
			std::vector<configServerBlock> server_blocks = config_obj.getServerBlocksObj();
			std::vector<configServerBlock>::iterator itata = server_blocks.begin();

			for (; itata != server_blocks.end(); itata++)
			{
				itata->printDirectives();
			}
		}

	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <stdlib.h>
#include <stdexcept>
#include "configFileTreatment.hpp"
#include "configServerBlock.hpp"

class configFile
{
    public:
        /****************/
        /* Constructors */
        /****************/
        configFile(char *configuration_file);
        ~configFile();

		std::vector<configServerBlock>		getServerBlocksObj() { return _server_blocks_obj; };

    private:
        /********/
        /* Data */
        /********/
        std::list<std::vector<std::string> >	_server_blocks; // This list contains the
																// server blocks.

		std::vector<configServerBlock>			_server_blocks_obj; // This vector contains
																	// all the server blocks
																	// treated (with IP, PORT,
																	// Root, Index...)
};

#endif

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

#define DEBUG 1

class configFile
{
    public:
        /****************/
        /* Constructors */
        /****************/
        configFile(char *configuration_file)
        {
            /*****************************************************************/
            /* Pass the configuration file to string _configuration_file_raw */
            /*****************************************************************/
            std::string					line;
			std::string					configuration_file_raw;
            std::ifstream				config_file(configuration_file);
            if (config_file.is_open())
            {
                while (getline(config_file, line))
                {
                    configuration_file_raw += line;
                    configuration_file_raw += "\n";
                }
                config_file.close();
            }
            else
            {
				throw std::runtime_error("configFile.hpp exception: configuration file not found or invalid");
            }

			if (configuration_file_raw.empty())
			{
				throw std::runtime_error("configFile.hpp exception: configuration file is empty");
			}

			/************************************************************/
            /* Treat the configuration file by removing # comments,		*/
			/* unnecessary spaces and tabs.								*/
			/* Check cases that have ; in same line.					*/
			/* If server blocks and locations have correct brackets.	*/
			/* If location blocks are within server blocks.				*/
			/* Putting on _configuration_file_treated.					*/
			/* Finally separating them in Server Blocks to				*/
			/* _server_blocks											*/
			/************************************************************/
			configFileTreatment treatment_obj(configuration_file_raw);

			treatment_obj.treatConfigurationFile();
			treatment_obj.separateToServerBlocks();
			_server_blocks = treatment_obj.getServerBlocks();

			if (DEBUG == 1)
				treatment_obj.printServerBlocks();
				//treatment_obj.printConfigurationFileTreated();

        };

        ~configFile()
        {};

    private:
        /********/
        /* Data */
        /********/
        std::list<std::vector<std::string> >	_server_blocks; // This list contains the
																// server blocks.
};

#endif

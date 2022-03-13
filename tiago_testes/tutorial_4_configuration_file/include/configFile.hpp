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

			/************************************************************/
			/* We now have server blocks separated in vectors<string>.  */
			/* Now we make an object (configServerBlock) for each of	*/
			/* the server blocks. These configServerBlock objects have  */
			/* all the informations of the server blocks.				*/
			/* Aftewards it will be pushed back to _server_blocks_obj.  */
			/************************************************************/

			std::list<std::vector<std::string> >::iterator it(_server_blocks.begin());

			for (; it != _server_blocks.end(); it++)
			{
				configServerBlock server_block_obj(*it);
				server_block_obj.fillBlocks();

				_server_blocks_obj.push_back(server_block_obj);
			}

			/************************************************************/
			/* We check on all the _server_blocks_obj if we have        */
			/* repeated ports and ips and then check if the server_name */
			/* is different. If they have the same name, we delete one  */
			/* of the server_block.										*/
			/************************************************************/
/*
			std::list<std::vector<std::string> >::iterator start(_server_blocks.begin());
			std::list<std::vector<std::string> >::iterator looper(_server_blocks.begin());

			for (; looper != _server_blocks.end(); looper++)
			{
				
				start++;
				if (start == _server_blocks.end())
					break ;
			}
*/

        };

        ~configFile()
        {};


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
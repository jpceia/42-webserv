/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 15:59:45 by tisantos          #+#    #+#             */
/*   Updated: 2022/03/26 17:08:37 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "configFile.hpp"

/****************/
/* Constructors */
/****************/
configFile::configFile(const std::string& configuration_file)
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
	/* We make a Default Config, to send to configServerBlock   */
	/* and to configLocationBlock.								*/
	/************************************************************/

	configDefaults config_default;
	config_default.fillDefaults();

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
		server_block_obj.fillBlocks(config_default);

		#ifdef DEBUG
			server_block_obj.printDirectives();
		#endif

		_server_blocks_obj.push_back(server_block_obj);
	}

	/************************************************************/
	/* We check on all the _server_blocks_obj if we have        */
	/* repeated ports and ips and then check if the server_name */
	/* is different. If they have the same name, we delete one  */
	/* of the server_block.										*/
	/************************************************************/
	std::vector<configServerBlock>::iterator start = _server_blocks_obj.begin();
	for (; start != _server_blocks_obj.end(); start++)
	{
		std::vector<configServerBlock>::iterator looper(_server_blocks_obj.begin());
		for (; looper != _server_blocks_obj.end(); looper++)
		{
			if (start != looper)
			{
				if ((start->getIP() 		== looper->getIP()) &&
					(start->getPort() 		== looper->getPort()) &&
					(start->getServerName() == looper->getServerName()))
					{
						std::cout << "Ignoring repeated server {" << start->getIP() << ":"
									<< start->getPort() << "} server_name = \"" << start->getServerName().front()
									<< "\"" << std::endl;
						_server_blocks_obj.erase(looper);
						start = _server_blocks_obj.begin();
						break ;
					}
			}
		}

	}

}

configFile::configFile(const configFile& rhs)
{
	*this = rhs;
}

configFile::~configFile()
{}

/*****************/
/* Copy Operator */
/*****************/
configFile& configFile::operator=(const configFile& rhs)
{
    if (this == &rhs)
    {
    	return *this;
    }
	_server_blocks = rhs._server_blocks;
	_server_blocks_obj = rhs._server_blocks_obj;

	return *this;
}

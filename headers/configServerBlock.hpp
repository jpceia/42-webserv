#ifndef CONFIGSERVERBLOCK_HPP
# define CONFIGSERVERBLOCK_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include "configLocationBlock.hpp"
#include "configDefaults.hpp"

class configServerBlock : public configDefaults
{
    public:
        configServerBlock();

        configServerBlock(std::vector<std::string> server_block);

        ~configServerBlock();

        void    fillBlocks();

		/****************/
		/* Print Helper */
		/****************/
		void	printDirectives();

		/*****************/
		/*    Getters    */
		/*****************/
		int									getLocationBlocksSize() { return (_location_blocks_count); }
		std::vector<configLocationBlock>	getLocationBlocks()
		{
			std::vector<configLocationBlock> send;

			for(int i = 0; i < _location_blocks_count; i++)
				send.push_back(_location_blocks[i]);

			return (send);
		}
		configLocationBlock					getLocationByPath(std::string path)
		{
			for (int i = 0; i < _location_blocks_count; i++)
			{
				if (_location_blocks[i].getLocationPath().front() == path)
					return _location_blocks[i];
			}
			return _location_blocks[0];
		}

		std::string							getIP() const					{ return (_ip.front()); }
		int									getPort() const					{ return (_port.front()); }
		std::string							getServerName() const			{ return (_server_name.front()); }

		std::multimap<int, std::string>		getErrorPage() const			{ return (_error_page); }
		std::vector<unsigned long int>		getClientMaxBodySize() const	{ return (_client_max_body_size); }
		std::vector<std::string>			getRoot() const					{ return (_root); }
		std::vector<std::string>			getIndex() const				{ return (_index); }
		std::vector<std::string>			getAutoIndex() const			{ return (_auto_index); }

    private:
        /********************/
        /* Helper Functions */
        /********************/
		void	fillServerBlock(std::string line);
		void	fillLocationBlock(std::string line);
		void	checkDuplicatedLocation();

		/************************/
		/* Directives Treatment */
		/************************/

		void	listenDirectiveTreatment(std::string line);
		void	listenCheckForDuplicates();

		void	servernameDirectiveTreatment(std::string line);
		void	errorpageDirectiveTreatment(std::string line);
		void	rootDirectiveTreatment(std::string line);
		void	clientmaxbodysizeDirectiveTreatment(std::string line);
		void	indexDirectiveTreatment(std::string line);
		void	autoindexDirectiveTreatment(std::string line);

		void	fillDirectivesIfEmpty();

        /********/
        /* Data */
        /********/
		std::vector<std::string>	_server_block;	// The server block you receive from
													// configFile.

		configLocationBlock			_location_blocks[1000]; // Location Blocks object treated
		int							_location_blocks_count; // How many location blocks on
															// _location_blocks[1000]

        /**********************/
        /* Current Directives */
        /**********************/

		// listen
		std::vector<std::string>			_ip;
		std::vector<int>					_port;

		std::vector<std::string>			_server_name;
		std::multimap<int, std::string>		_error_page;
        std::vector<unsigned long int>		_client_max_body_size;
        std::vector<std::string>			_root;
        std::vector<std::string>			_index;
        std::vector<std::string>			_auto_index;
};

#endif

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
			/************************************/
			/* If there is an exact path match. */
			/* path = /hello/world         <--- */
			/* location[0] = /hello/world  <--- */
			/* location[1] = /goodbye           */
			/* location[2] = /goodbye/alo       */
			/************************************/
			for (int i = 0; i < _location_blocks_count; i++)
			{
				if (_location_blocks[i].getLocationPath() == path)
					return _location_blocks[i];
			}

			/**************************************/
			/* If no match, find the closest      */
			/* path = /goodbye/year        <---   */
			/* location[0] = /hello/world         */
			/* location[1] = /goodbye      <---   */
			/* location[2] = /goodbye/alo         */
			/**************************************/

			// Divide the path with / as delimiter, and put the split on
			// a vector.
			std::vector<std::string> path_vector = returnSplitedDelimiter(path, "/");

			// Traverse all paths of location_blocks
			// Check which is the closest match
			std::vector<std::string> location_path;
			int		count_words = 0;
			int		location_to_return = 0;
			for (int i = 0; i < _location_blocks_count; i++)
			{
				int	count_words_temp = 0;
				location_path = returnSplitedDelimiter(_location_blocks[i].getLocationPath(), "/");

				std::vector<std::string>::iterator path_it = path_vector.begin();
				std::vector<std::string>::iterator loc_it  = location_path.begin();
				for (; loc_it != location_path.end(); loc_it++, path_it++)
				{
					if (*path_it == *loc_it)
					{
						count_words_temp++;
					}
					else
						break ;
				}

				if (count_words_temp > count_words)
				{
					location_to_return = i;
					count_words = count_words_temp;
				}
			}
			if (count_words > 0)
				return _location_blocks[location_to_return];
			else
				return _location_blocks[0];
		}

		std::string							getIP() const					{ return (_ip.front()); }
		int									getPort() const					{ return (_port.front()); }
		std::vector<std::string>			getServerName() const			{ return (_server_name); }

		std::map<int, std::string>			getErrorPage() const			{ return (_error_page); }
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

		std::vector<std::string>	returnSplitedDelimiter(std::string line,
														   std::string delimiter);
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
		std::map<int, std::string>			_error_page;
        std::vector<unsigned long int>		_client_max_body_size;
        std::vector<std::string>			_root;
        std::vector<std::string>			_index;
        std::vector<std::string>			_auto_index;
};

#endif

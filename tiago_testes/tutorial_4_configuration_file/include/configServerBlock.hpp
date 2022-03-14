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
		configLocationBlock					*getLocationByPath(std::string path)
		{
			for (int i = 0; i < _location_blocks_count; i++)
			{
				if (_location_blocks[i].getLocationPath().front() == path)
					return &_location_blocks[i];
			}
			return NULL;
		}

		std::string							getIP() const					{ return (_ip.front()); }
		int									getPort() const					{ return (_port.front()); }
		std::vector<std::string>			getServerName() const			{ return (_server_name); }

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

<<<<<<< HEAD
			if (_root.empty())
			{
				_error_path.push_back(*_root_default.begin() + path);
			}
			else
			{
				_error_path.push_back(*_root.begin() + path);
			}

			std::stringstream	is2(line);
			int					i = 0;
			_error_status.clear();
			while (is2 >> word)
			{
				if (i  != number_of_words - 1 && i != 0)
				{
					if ((word.find_first_not_of("0123456789") == std::string::npos) == false)
					{
						throw std::runtime_error("configServerBlock.hpp exception: error_page has invalid status code");
					}
					else
					{
						int status_code = atoi(word.c_str());
						if (status_code < 300 || status_code > 599)
						{
							throw std::runtime_error("configServerBlock.hpp exception: error_page has invalid range status code");
						}
						else
						{
							if (std::find(_error_status.begin(), _error_status.end(), status_code) != _error_status.end())
							{}
							else
								_error_status.push_back(status_code);
						}
					}
				}
				i++;
			}
		}
		void	rootDirectiveTreatment(std::string line)
		{
			/***********************************************************/
			/* Ignore the first word and add the rest of the arguments */
			/* to the _root vector.									   */
			/* Check for duplicates so it doesn't add duplicates 	   */
			/* (throw).												   */
			/* Check for ';' if it's one argument only.				   */
			/* Check for ';' at the end and remove it.				   */
			/***********************************************************/
			std::stringstream	is(line);
			std::string			word;
			int					number_of_words = 0;
			while (is >> word)
			{
				if (number_of_words == 0)
				{
					number_of_words++;
					continue ;
				}
				if (number_of_words == 1 && !word.compare(";"))
				{
					throw std::runtime_error("configServerBlock.hpp exception: root has no arguments");
				}
				if (number_of_words > 1)
				{
					throw std::runtime_error("configServerBlock.hpp exception: root has too many arguments");
				}
				if (*word.rbegin() == ';')
				{
					word.resize(word.size() - 1);
				}
				if (_root.empty())
				{
					_root.push_back(word);
				}
				else
				{
					throw std::runtime_error("configServerBlock.hpp exception: root has duplicated value");
				}
				number_of_words++;
			}
		}
		void	clientmaxbodysizeDirectiveTreatment(std::string line)
		{
			/***********************************************************/
			/* Ignore the first word and treat the second word.		   */
			/* Check if it has more than 1 argument and throw if it    */
			/* has.													   */
			/* Check for duplicates so it doesn't add duplicates 	   */
			/* (throw).												   */
			/* Check for ';' if it's one argument only.				   */
			/* Check for ';' at the end and remove it.				   */
			/* Check for last letter if it's a k K g G m M			   */
			/* Do the conversions.									   */
			/***********************************************************/
			std::stringstream	is(line);
			std::string			word;
			char				size_type;
			long int		client_max_body_size = 0;
			int					number_of_words = 0;
			while (is >> word)
			{
				if (number_of_words == 0)
				{
					number_of_words++;
					continue ;
				}
				if (number_of_words == 1 && !word.compare(";"))
				{
					throw std::runtime_error("configServerBlock.hpp exception: client_max_body_size has no arguments");
				}
				if (number_of_words > 1)
				{
					throw std::runtime_error("configServerBlock.hpp exception: client_max_body_size has too many arguments");
				}
				if (*word.rbegin() == ';')
				{
					word.resize(word.size() - 1);
				}
				if (*word.rbegin() == 'k' || *word.rbegin() == 'K' ||
					*word.rbegin() == 'g' || *word.rbegin() == 'G' ||
					*word.rbegin() == 'm' || *word.rbegin() == 'M')
				{
					size_type = *word.rbegin();
					word.resize(word.size() - 1);
				}
				if ((word.find_first_not_of("0123456789") == std::string::npos) == false)
				{
					throw std::runtime_error("configServerBlock.hpp exception: client_max_body_size has invalid argument");
				}
				if (size_type == 'K' || size_type == 'k')
					client_max_body_size = atoll(word.c_str()) * 1024;
				else if (size_type == 'M' || size_type == 'm')
					client_max_body_size = atoll(word.c_str()) * (1024 * 1024);
				else if (size_type == 'G' || size_type == 'g')
				{
					client_max_body_size = atoll(word.c_str()) * (1024 * 1024 * 1024);
				}
				else
					client_max_body_size = atoll(word.c_str());
				if (_client_max_body_size.empty())
				{
					_client_max_body_size.push_back(client_max_body_size);
				}
				else
				{
					throw std::runtime_error("configServerBlock.hpp exception: client_max_body_size has duplicated value");
				}
				number_of_words++;
			}
		}
		void	indexDirectiveTreatment(std::string line)
		{
			/***********************************************************/
			/* Ignore the first word and add the rest of the arguments */
			/* to the _index vector.	 							   */
			/* Check for duplicates so it doesn't add duplicates 	   */
			/* (not a throw).										   */
			/* Check for ';' if it's one argument only.				   */
			/* Check for ';' at the end and remove it.				   */
			/***********************************************************/
			std::stringstream	is(line);
			std::string			word;
			int					number_of_words = 0;
			while (is >> word)
			{
				if (number_of_words == 0)
				{
					number_of_words++;
					continue ;
				}
				if (number_of_words == 1 && !word.compare(";"))
				{
					throw std::runtime_error("configServerBlock.hpp exception: index has no arguments");
				}
				if (*word.rbegin() == ';')
				{
					word.resize(word.size() - 1);
				}
				if (std::find(_index.begin(), _index.end(), word) != _index.end())
				{}
				else
				{
					if (!_root.empty())
					{
						_index.push_back(*_root.begin() + "/" + word);
					}
					else
					{
						_index.push_back(*_root_default.begin() + "/" + word);
					}
				}
				number_of_words++;
			}
		}
		void	autoindexDirectiveTreatment(std::string line)
		{
			/***********************************************************/
			/* Ignore the first word and treat the second word.		   */
			/* Check if it has more than 1 argument and throw if it    */
			/* has.													   */
			/* Check for duplicates so it doesn't add duplicates 	   */
			/* (throw).												   */
			/* Check for ';' if it's one argument only.				   */
			/* Check for ';' at the end and remove it.				   */
			/***********************************************************/
			std::stringstream	is(line);
			std::string			word;
			int					number_of_words = 0;
			while (is >> word)
			{
				if (number_of_words == 0)
				{
					number_of_words++;
					continue ;
				}
				if (number_of_words == 1 && !word.compare(";"))
				{
					throw std::runtime_error("configServerBlock.hpp exception: auto_index has no arguments");
				}
				if (number_of_words > 1)
				{
					throw std::runtime_error("configServerBlock.hpp exception: auto_index has too many arguments");
				}
				if (*word.rbegin() == ';')
				{
					word.resize(word.size() - 1);
				}
				if (word.compare("on") && word.compare("off"))
				{
					throw std::runtime_error("configServerBlock.hpp exception: auto_index has invalid argument");
				}
				if (_auto_index.empty())
				{
					_auto_index.push_back(word);
				}
				else
				{
					throw std::runtime_error("configServerBlock.hpp exception: auto_index has duplicated value");
				}
				number_of_words++;
			}
		}

		void	fillDirectivesIfEmpty(configServerBlock & obj)
		{
			if (_server_name.empty())
				_server_name = _server_name_default;
			else if (_client_max_body_size.empty())
				_client_max_body_size = _client_max_body_size_default;
			else if (_root.empty())
				_root = _root_default;
			/* */
		}
=======
		void	fillDirectivesIfEmpty();
>>>>>>> feature-config_files_change

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

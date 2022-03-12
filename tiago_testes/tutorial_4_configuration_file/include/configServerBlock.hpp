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

class configServerBlock
{
    public:
        configServerBlock()
        {};

        configServerBlock(std::vector<std::string> server_block) : _server_block(server_block)
        {
			fillDefaultConfigurations();
        };

        ~configServerBlock()
        {};

        void    fillBlocks()
        {
 			std::vector<std::string>::iterator it(_server_block.begin());
			int		on_server			= 0;
			int		on_location			= 0;
			_location_blocks_count		= 0; // How many location blocks are there.

			for (;it != _server_block.end(); it++)
			{
				std::string	line;
				int i = 0;

				std::istringstream iss(*it);
				while (iss)
				{
					std::string subs;
					iss >> subs;

					if (i == 0 && !subs.compare("server"))
					{
						on_server = 1;
					}
					else if (i == 0 && !subs.compare("location"))
					{
						on_server = 0;
						on_location = 1;
						_location_blocks_count++;
					}

					line += subs;
					line += " ";

					i++;
				}
				if (line.size() > 1)
					line.resize(line.size() - 2);

				if (!line.compare("}"))
				{
					on_location = 0;
				}

				if (on_server == 1)
				{
					fillServerBlock(line);
				}
				else if (on_location == 1)
				{
					fillLocationBlock(line);
				}
			}

			/**************************************************/
			/* If listen wasn't found, fill it manually.      */
			/* If server_name wasn't found, fill it manually. */
			/* If root wasn't found, fill it manually.        */
			/**************************************************/
			if (_ip.empty() && _port.empty())
			{
				_ip = _ip_default;
				_port = _port_default;
			}
			if (_server_name.empty())
				_server_name = _server_name_default;
			if (_root.empty())
				_root = _root_default;

			printDirectives();
		};

		/****************/
		/* Print Helper */
		/****************/
		void	printDirectives()
		{
			/**********************************/
			/*			  listen              */
			/**********************************/
			std::cout << "Listen:" << std::endl;
			std::vector<std::string>::iterator	ip_it = _ip.begin();
			std::vector<int>::iterator	port_it = _port.begin();
			for (; ip_it != _ip.end(); ip_it++, port_it++)
			{
				std::cout << "  { " << *ip_it << ":" << *port_it << " }" << std::endl;
			}

			/**********************************/
			/*			  server_name         */
			/**********************************/
			std::cout << "Server_name:" << std::endl;
			std::vector<std::string>::iterator	server_name_it = _server_name.begin();
			std::cout << "  { ";
			for (; server_name_it != _server_name.end(); server_name_it++)
			{
				std::cout <<  *server_name_it << " ";
			}
			std::cout << "}" << std::endl;

			/**********************************/
			/*			  error_page          */
			/**********************************/
			if (!_error_status.empty() && !_error_path.empty())
			{
				std::cout << "Error_page:" << std::endl;
				std::vector<int>::iterator	_error_status_it = _error_status.begin();
				std::cout << "  { ";
				for (; _error_status_it != _error_status.end(); _error_status_it++)
				{
					std::cout <<  *_error_status_it << " ";
				}
				std::cout << *_error_path.begin();
				std::cout << "}" << std::endl;
			}

			/**********************************/
			/*			    root              */
			/**********************************/
			std::cout << "Root:" << std::endl;
			std::cout << "  { " << *_root.begin() << " }" << std::endl;


		}


    private:
        /********************/
        /* Helper Functions */
        /********************/
		void	fillServerBlock(std::string line)
		{
			std::istringstream	iss(line);
			while (iss)
			{
				std::string subs;
				iss >> subs;

				if (!subs.compare("server"))
				{}
				else if (!subs.compare("listen"))
					listenDirectiveTreatment(line);
				else if (!subs.compare("server_name"))
					servernameDirectiveTreatment(line);
				else if (!subs.compare("error_page"))
					errorpageDirectiveTreatment(line);
				else if (!subs.compare("client_max_body_size"))
				{}
				else if (!subs.compare("root"))
					rootDirectiveTreatment(line);
				else if (!subs.compare("index"))
				{}
				else if (!subs.compare("autoindex"))
				{}
				else if (!subs.compare("}"))
				{}
				else
				{
					std::cerr << "configServerBlock.hpp exception: " << subs << " not valid on server block";
					throw std::runtime_error("");
				}
				return ;
			}
		}

		void	fillLocationBlock(std::string line)
		{
			//std::cout << "LOCATION BLOCK = ";
			//std::cout << line << std::endl;

			std::istringstream	iss(line);
			while (iss)
			{
				std::string subs;
				iss >> subs;

				if (!subs.compare("location"))
				{}
				else if (!subs.compare("error_page"))
				{}
				else if (!subs.compare("client_max_body_size"))
				{}
				else if (!subs.compare("root"))
				{}
				else if (!subs.compare("index"))
				{}
				else if (!subs.compare("autoindex"))
				{}
				else if (!subs.compare("return"))
				{}
				else if (!subs.compare("methods"))
				{}
				else if (!subs.compare("cgi"))
				{}
				else if (!subs.compare("upload"))
				{}
				else if (!subs.compare("}"))
				{}
				else
				{
					std::cerr << "configServerBlock.hpp exception: " << subs << " not valid on location block";
					throw std::runtime_error("");
				}
				return ;
			}
		}


		/************************/
		/* Directives Treatment */
		/************************/

		void	listenDirectiveTreatment(std::string line)
		{
			/*****************************************************************/
			/* First count how many args.     								 */
			/* Extract the first argument if it has.						 */
			/* listen can only have 1 arg which is the IP and PORT together. */
			/*****************************************************************/
			std::stringstream	is(line);
			std::string			word;
			std::string			argument;
			int					number_of_words = 0;
			while (is >> word)
			{
				if (number_of_words == 1)
					argument = word;
				number_of_words++;
			}

			if (number_of_words != 2)
			{
				throw std::runtime_error("configServerBlock.hpp exception: listen has wrong number of args");
			}

			/*****************************************************************/
			/* Argument was extracted previously.							 */
			/* First check if it's just a ;									 */
			/* Then remove the ending ';'									 */
			/* If it has a : divide it in two.								 */
			/* If it doesn't have a : define if it's a PORT or IP.			 */
			/* IP can be "localhost" or an IP like (0.0.0.0).				 */
			/* PORT can only be an integer.									 */
			/*****************************************************************/
			if (!argument.compare(";"))
			{
				throw std::runtime_error("configServerBlock.hpp exception: listen has no arguments");
			}

			argument.resize(argument.size() - 1);

			std::string	ip_extracted;
			std::string	port_extracted;

			int pos = argument.find_last_of(':');
			if (pos > 0)
			{
				ip_extracted = argument.substr(0, pos);
				port_extracted = argument.substr(pos+1);

				if ((port_extracted.find_first_not_of("0123456789") == std::string::npos) == false)
				{
					throw std::runtime_error("configServerBlock.hpp exception: listen has invalid PORT argument");
				}

				if (ip_extracted.find_first_of('.') == 0)
				{
					if (ip_extracted.compare("localhost"))
					{
						throw std::runtime_error("configServerBlock.hpp exception: listen has invalid IP argument");
					}
				}
				else
				{
					_ip.push_back(ip_extracted);
					_port.push_back(atoi(port_extracted.c_str()));
					listenCheckForDuplicates();
					return ;
				}
				throw std::runtime_error("configServerBlock.hpp exception: listen has invalid IP argument");
			}

			if (argument.find_first_of('.') == 0)
			{
				if (!argument.compare("localhost"))
				{
					_ip.push_back("localhost");
					_port.push_back(80);
					listenCheckForDuplicates();
					return ;
				}
				else if ((argument.find_first_not_of("0123456789") == std::string::npos) == false)
				{
					throw std::runtime_error("configServerBlock.hpp exception: listen has invalid IP or PORT argument");
				}
				else
				{
					_ip.push_back("0.0.0.0");
					_port.push_back(atoi(argument.c_str()));
					listenCheckForDuplicates();
				}
			}
			else
			{
				if ((argument.find_first_not_of("0123456789") == std::string::npos) == false)
				{
					_ip.push_back(argument);
					_port.push_back(80);
					listenCheckForDuplicates();
					return ;
				}
				else
				{
					_ip.push_back("0.0.0.0");
					_port.push_back(atoi(argument.c_str()));
					listenCheckForDuplicates();
				}
			}
		}
		void	listenCheckForDuplicates()
		{
			std::vector<std::string>::iterator	ip_it = _ip.begin();
			std::vector<int>::iterator	port_it = _port.begin();
			for (int i = 0; ip_it != _ip.end(); ip_it++, port_it++)
			{
				std::vector<std::string>::iterator	ip_compare = _ip.begin();
				std::vector<int>::iterator	port_compare = _port.begin();
				for (int x = 0; ip_compare != _ip.end(); ip_compare++, port_compare++)
				{
					if (i != x)
					{
						if (!ip_it->compare(*ip_compare) && *port_it == *port_compare)
							throw std::runtime_error("configServerBlock.hpp exception: listen has duplicated value");
					}
					x++;
				}
				i++;
			}
		}

		void	servernameDirectiveTreatment(std::string line)
		{
			/***********************************************************/
			/* Ignore the first word and add the rest of the arguments */
			/* to the _server_name vector. 							   */
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
					throw std::runtime_error("configServerBlock.hpp exception: server_name has no arguments");
				}
				if (*word.rbegin() == ';')
				{
					word.resize(word.size() - 1);
				}
				if (std::find(_server_name.begin(), _server_name.end(), word) != _server_name.end())
				{}
				else
					_server_name.push_back(word);
				number_of_words++;
			}
		}
		void	errorpageDirectiveTreatment(std::string line)
		{
			/*****************************************************************/
			/* First count how many args.     								 */
			/* Check if it has more than 1 args. 							 */
			/* Check if it's empty.											 */
			/*****************************************************************/
			std::stringstream	is(line);
			std::string			word;
			std::string			path;
			int					number_of_words = 0;
			while (is >> word)
			{
				path = word;
				number_of_words++;
			}

			if (number_of_words == 2 && !word.compare(";"))
			{
				throw std::runtime_error("configServerBlock.hpp exception: error_page has no arguments");
			}

			if (number_of_words < 3)
			{
				throw std::runtime_error("configServerBlock.hpp exception: error_page has wrong number of args");
			}

			/*****************************************************************/
			/* Remove ';' from path.										 */
			/* Add root to the path.										 */
			/* Check if values are between 300 and 599.						 */
			/* Add everything to error status, error path.					 */
			/*****************************************************************/
			path.resize(path.size() - 1);
			_error_path.clear();

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
			/* to the _server_name vector. 							   */
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


        void    fillDefaultConfigurations()
        {
			// listen
			_ip_default.push_back("0.0.0.0");
			_port_default.push_back(80);

            _server_name_default.push_back("");
            _client_max_body_size_default.push_back("1000000");
            _root_default.push_back("/www/html");            // These needs to be checked
            _index_default.push_back("index.html");       // These needs to be checked
            _auto_index_default.push_back("off");

            _methods_default.push_back("GET");
            _upload_default.push_back("off");
        };

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
		std::vector<std::string>	_ip;
		std::vector<int>			_port;

		std::vector<std::string>    _server_name;

		// error_page
        std::vector<int>            _error_status;
        std::vector<std::string>    _error_path;

        std::vector<std::string>    _client_max_body_size;
        std::vector<std::string>    _root;
        std::vector<std::string>    _index;
        std::vector<std::string>    _auto_index;

        std::vector<std::string>    _methods;
        std::vector<std::string>    _redirect;
        std::vector<std::string>    _cgi;
        std::vector<std::string>    _upload;

        /**********************/
        /* Default Directives */
        /**********************/

		// listen
		std::vector<std::string>	_ip_default;
		std::vector<int>			_port_default;

		std::vector<std::string>    _server_name_default;

        std::vector<std::string>    _client_max_body_size_default;
        std::vector<std::string>    _root_default;
        std::vector<std::string>    _index_default;
        std::vector<std::string>    _auto_index_default;

        std::vector<std::string>    _methods_default;
        std::vector<std::string>    _upload_default;
};

#endif

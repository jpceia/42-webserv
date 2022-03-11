#ifndef CONFIGSERVERBLOCK_HPP
# define CONFIGSERVERBLOCK_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <string>

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
			int		on_server		= 0;
			int		on_location		= 0;

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
		};


    private:
        /********************/
        /* Helper Functions */
        /********************/
		void	fillServerBlock(std::string line)
		{
			std::cout << "SERVER BLOCK ";
			std::cout << line << std::endl;
		}

		void	fillLocationBlock(std::string line)
		{
			std::cout << "LOCATION BLOCK ";
			std::cout << line << std::endl;
		}


        void    fillDefaultConfigurations()
        {
			// listen
            _ip_default.push_back("0.0.0.0");
            _port_default.push_back("80");

            _server_name_default.push_back("");
            _error_page_default.push_back("");
            _client_max_body_size_default.push_back("1000000");
            _root_default.push_back("./www/");            // These needs to be checked
            _index_default.push_back("index.html");       // These needs to be checked
            _auto_index_default.push_back("off");

            _methods_default.push_back("GET");
            _redirect_default.push_back("");
            _cgi_default.push_back("");
            _upload_default.push_back("off");
        };

        /********/
        /* Data */
        /********/
		std::vector<std::string>	_server_block;	// The server block you receive from
													// configFile.

        /**********************/
        /* Current Directives */
        /**********************/

		// listen
        std::vector<std::string>    _ip;
        std::vector<std::string>    _port;

		std::vector<std::string>    _server_name;
        std::vector<std::string>    _error_page;
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
        std::vector<std::string>    _ip_default;
        std::vector<std::string>    _port_default;

		std::vector<std::string>    _server_name_default;
        std::vector<std::string>    _error_page_default;
        std::vector<std::string>    _client_max_body_size_default;
        std::vector<std::string>    _root_default;
        std::vector<std::string>    _index_default;
        std::vector<std::string>    _auto_index_default;

        std::vector<std::string>    _methods_default;
        std::vector<std::string>    _redirect_default;
        std::vector<std::string>    _cgi_default;
        std::vector<std::string>    _upload_default;
};

#endif

#ifndef CONFIGLOCATIONBLOCK_HPP
# define CONFIGLOCATIONBLOCK_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <stdexcept>
#include <algorithm>

class configLocationBlock
{
    public:
        configLocationBlock()
        {};

        configLocationBlock(std::vector<std::string> location_block) : _location_block(location_block)
        {
			fillDefaultConfigurations();
        };

        ~configLocationBlock()
        {};


    private:
        /********************/
        /* Helper Functions */
        /********************/


        void    fillDefaultConfigurations()
        {
			// listen
			_ip_default.push_back("0.0.0.0");
			_port_default.push_back(80);

            _server_name_default.push_back("");

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
		std::vector<std::string>	_location_block;	// The location block you receive from
														// configFile.

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
        std::vector<std::string>    _redirect_default;
        std::vector<std::string>    _cgi_default;
        std::vector<std::string>    _upload_default;
};

#endif

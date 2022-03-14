#ifndef CONFIGDEFAULTS_HPP
# define CONFIGDEFAULTS_HPP

#include <iostream>
#include <vector>

class configDefaults
{
	public:
		configDefaults();
		~configDefaults();

        /**********************/
        /* Default Directives */
        /**********************/
		// listen
		std::vector<std::string>		_ip_default;
		std::vector<int>				_port_default;

		std::vector<std::string>		_server_name_default;

        std::vector<unsigned long int>	_client_max_body_size_default;
        std::vector<std::string> 		_root_default;
        std::vector<std::string>   		_index_default;
        std::vector<std::string>   		_auto_index_default;

        std::vector<std::string>   		_methods_default;
        std::vector<std::string>  		_upload_default;
};

#endif

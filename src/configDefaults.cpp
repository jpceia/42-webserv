/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configDefaults.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 15:59:40 by tisantos          #+#    #+#             */
/*   Updated: 2022/03/25 15:38:06 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "configDefaults.hpp"
# include <sstream>
# include <fstream>
# include <cstdlib>

/****************/
/* Constructors */
/****************/
configDefaults::configDefaults()
{
	_ip_default.push_back("0.0.0.0");
	_port_default.push_back(80);

	_server_name_default.push_back("");
	_client_max_body_size_default.push_back(1000000);
	_root_default.push_back("/www/html");            // These needs to be checked
	_index_default.push_back("/index.html");       	 // These needs to be checked
	_auto_index_default.push_back("off");

	_methods_default.insert(GET);
	_upload_default.push_back("");


			//std::cout << "HERE\n";


	/*****************************************/
	/* Read the /defaults/default_config.txt */
	/* Parse the file and set it to 		 */
	/* error_page default.					 */
	/*****************************************/

	std::string 	line;
	std::ifstream 	ifs("./defaults/default_config.txt");

	if (ifs.is_open())
	{
		while (getline(ifs, line))
		{
			size_t pos = 0;
			std::string path_temp = line;
			std::vector<std::string> path_vector;
			while ((pos = path_temp.find(" ")) != std::string::npos)
			{
				path_vector.push_back(path_temp.substr(0, pos));
				path_temp.erase(0, pos + 1);
			}
			path_vector.push_back(path_temp);

			_error_page.insert(std::pair<int, std::string>(atoi(path_vector.front().c_str()), *path_vector.rbegin()));
		}
		ifs.close();

/*		std::map<int, std::string>::iterator	_error_page_it = _error_page.begin();
		for (int i = 0; _error_page_it != _error_page.end(); _error_page_it++, i++)
		{
			if (i == 0)
				std::cout << "		  { ";
			else
				std::cout << "			  { ";
			std::cout <<  _error_page_it->first << " " << _error_page_it->second;
			std::cout << " }" << std::endl;
		}
*/

	}
	else
	{
		throw std::runtime_error("default_config.txt not found or directory incorrect. Should be at /webserv/defaults/default_config.txt");
	}

}

configDefaults::configDefaults(const configDefaults& rhs)
{
	*this = rhs;
}

configDefaults::~configDefaults()
{}

/*****************/
/* Copy Operator */
/*****************/
configDefaults& configDefaults::operator=(const configDefaults& rhs)
{
    if (this == &rhs)
    {
    	return *this;
    }
	_ip_default = rhs._ip_default;
	_port_default = rhs._port_default;
	_server_name_default = rhs._server_name_default;
    _client_max_body_size_default = rhs._client_max_body_size_default;
    _root_default = rhs._root_default;
    _index_default = rhs._index_default;
    _auto_index_default = rhs._auto_index_default;
    _methods_default = rhs._methods_default;
    _upload_default = rhs._upload_default;
	_error_page = rhs._error_page;

	return *this;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configDefaults.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 15:59:40 by tisantos          #+#    #+#             */
/*   Updated: 2022/03/19 16:47:14 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "configDefaults.hpp"

/****************/
/* Constructors */
/****************/
configDefaults::configDefaults()
{
	// listen
	_ip_default.push_back("0.0.0.0");
	_port_default.push_back(80);

	_server_name_default.push_back("");
	_client_max_body_size_default.push_back(1000000);
	_root_default.push_back("/www/html");            // These needs to be checked
	_index_default.push_back("/index.html");       	 // These needs to be checked
	_auto_index_default.push_back("off");

	_methods_default.push_back("GET");
	_upload_default.push_back("");
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
	
	return *this;
}

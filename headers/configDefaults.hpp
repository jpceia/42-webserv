/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configDefaults.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 15:58:50 by tisantos          #+#    #+#             */
/*   Updated: 2022/03/25 17:51:04 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGDEFAULTS_HPP
# define CONFIGDEFAULTS_HPP

# include <iostream>
# include <vector>
# include <map>
# include <set>
# include "HTTPMethod.hpp"

#ifndef DEFAULTS_PATH
# define DEFAULTS_PATH "./defaults/default_config.txt"
#endif

class configDefaults
{
	public:
        /****************/
        /* Constructors */
        /****************/
		configDefaults();
        configDefaults(const configDefaults& rhs);
		~configDefaults();

        /*****************/
        /* Copy Operator */
        /*****************/
        configDefaults& operator=(const configDefaults& rhs);

        /***********/
        /* Methods */
        /***********/
		void fillDefaults();

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

        std::set<HTTPMethod>   		    _methods_default;
        std::vector<std::string>  		_upload_default;

        std::map<int, std::string>      _error_page_default;
};

#endif

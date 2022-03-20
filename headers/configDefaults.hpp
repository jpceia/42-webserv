/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configDefaults.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 15:58:50 by tisantos          #+#    #+#             */
/*   Updated: 2022/03/20 02:32:20 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGDEFAULTS_HPP
# define CONFIGDEFAULTS_HPP

# include <iostream>
# include <vector>
# include <set>
# include "HTTPMethod.hpp"

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
};

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 15:58:56 by tisantos          #+#    #+#             */
/*   Updated: 2022/03/26 17:08:57 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <stdlib.h>
#include <stdexcept>
#include "configFileTreatment.hpp"
#include "configServerBlock.hpp"

class configFile
{
    public:
        /****************/
        /* Constructors */
        /****************/
        configFile(const std::string& configuration_file);
        configFile(const configFile& rhs);
        ~configFile();

        /*****************/
        /* Copy Operator */
        /*****************/
        configFile& operator=(const configFile& rhs);

        /***********/
        /* Getters */
        /***********/
		std::vector<configServerBlock>		getServerBlocksObj() { return _server_blocks_obj; };

    private:
        /********/
        /* Data */
        /********/
        std::list<std::vector<std::string> >	_server_blocks; // This list contains the
																// server blocks.

		std::vector<configServerBlock>			_server_blocks_obj; // This vector contains
																	// all the server blocks
																	// treated (with IP, PORT,
																	// Root, Index...)
};

#endif

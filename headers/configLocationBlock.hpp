/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configLocationBlock.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 15:59:08 by tisantos          #+#    #+#             */
/*   Updated: 2022/03/25 17:08:03 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGLOCATIONBLOCK_HPP
# define CONFIGLOCATIONBLOCK_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <set>
# include <sstream>
# include <string>
# include <stdexcept>
# include <algorithm>
# include "configDefaults.hpp"
# include "HTTPMethod.hpp"

class configLocationBlock
{
    public:
        /****************/
        /* Constructors */
        /****************/
        configLocationBlock();
        configLocationBlock(const configLocationBlock& rhs);
        ~configLocationBlock();

        /*****************/
        /* Copy Operator */
        /*****************/
        configLocationBlock& operator=(const configLocationBlock& rhs);

        /***********/
        /* Methods */
        /***********/
		void	fillDefaultValues(configDefaults default_values);

		void	locationDirectiveTreatment(std::string line);
		void	errorpageDirectiveTreatment(std::map<int, std::string> error_p,
											std::string line);
		void	clientmaxbodysizeDirectiveTreatment(std::string line);
		void	rootDirectiveTreatment(std::string line);
		void	indexDirectiveTreatment(std::vector<std::string> ind, std::string line);
		void	autoindexDirectiveTreatment(std::string line);
		void	returnDirectiveTreatment(std::string line);
		void	methodsDirectiveTreatment(std::string line);
		void	cgiDirectiveTreatment(std::string line);
		void	uploadDirectiveTreatment(std::string line);
		void	fillDirectivesIfEmpty( std::vector<unsigned long int> client_max_body_size,
									   std::vector<std::string> root,
									   std::vector<std::string> autoindex,
									   std::vector<std::string> index,
									   std::map<int, std::string> errorpage);

		/*****************/
		/*    Getters    */
		/*****************/
		std::string								getLocationPath() const				{ return (_location_path.size() > 0 ? _location_path.front() : ""); }
		std::map<int, std::string>				getErrorPage() const				{ return (_error_page); }
		std::string								getRoot() const						{ return (_root.size() > 0 ? _root.front() : ""); }
		unsigned long int						getClientMaxBodySize() const		{ return (_client_max_body_size.size() > 0 ? _client_max_body_size.front() : 0); }
		std::vector<std::string>				getIndex() const					{ return (_index); }
		std::string								getAutoIndex() const				{ return (_auto_index.size() > 0 ? _auto_index.front() : ""); }
		std::set<HTTPMethod>					getMethods() const					{ return (_methods); }
		int										getRedirectStatus() const			{ return (_redirect_status.size() > 0 ? _redirect_status.front() : 0); }
		std::string								getRedirectPath() const				{ return (_redirect_path.size() > 0 ? _redirect_path.front() : ""); }
		std::map<std::string, std::string>		getCgi() const						{ return (_cgi); }
		std::string								getUpload() const					{ return (_upload.size() > 0 ? _upload.front() : ""); }
		bool									getIsRootFromLocationBlock() const	{ return (_isrootfromlocationblock); }

    public:
        /**********************/
        /* Current Directives */
        /**********************/
		std::vector<std::string>				_location_path;
		std::map<int, std::string>				_error_page;
        std::vector<unsigned long int>			_client_max_body_size;
        std::vector<std::string>    			_root;
        std::vector<std::string>    			_index;
        std::vector<std::string>    			_auto_index;
		std::set<HTTPMethod>					_methods;
		std::vector<int>            			_redirect_status;
        std::vector<std::string>    			_redirect_path;
        std::map<std::string, std::string>		_cgi;
        std::vector<std::string>    			_upload;
		bool									_isrootfromlocationblock;

		configDefaults							_default_values;		// Default Values
};

#endif

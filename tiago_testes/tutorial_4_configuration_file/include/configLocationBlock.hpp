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
#include "configDefaults.hpp"

class configLocationBlock : public configDefaults
{
    public:
        configLocationBlock();

        ~configLocationBlock();

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
									   std::vector<std::string> autoindex);

		/*****************/
		/*    Getters    */
		/*****************/
		std::vector<std::string>				getLocationPath() const			{ return (_location_path); }
		std::map<int, std::string>				getErrorPage() const			{ return (_error_page); }
		std::vector<unsigned long int>			getClientMaxBodySize() const	{ return (_client_max_body_size); }
		std::vector<std::string>				getRoot() const					{ return (_root); }
		std::vector<std::string>				getIndex() const				{ return (_index); }
		std::vector<std::string>				getAutoIndex() const			{ return (_auto_index); }
		std::vector<std::string>				getMethods() const				{ return (_methods); }
		std::vector<int>						getRedirectStatus() const		{ return (_redirect_status); }
		std::vector<std::string>				getRedirectPath() const			{ return (_redirect_path); }
		std::map<std::string, std::string>	getCgi() const						{ return (_cgi); }
		std::vector<std::string>				getUpload() const				{ return (_upload); }

    private:
        /**********************/
        /* Current Directives */
        /**********************/

		// location_path
		std::vector<std::string>				_location_path;

		std::map<int, std::string>				_error_page;
        std::vector<unsigned long int>			_client_max_body_size;
        std::vector<std::string>    			_root;
        std::vector<std::string>    			_index;
        std::vector<std::string>    			_auto_index;

        std::vector<std::string>    			_methods;

		// return (redirect)
		std::vector<int>            			_redirect_status;
        std::vector<std::string>    			_redirect_path;

        std::map<std::string, std::string>		_cgi;
        std::vector<std::string>    			_upload;
};

#endif

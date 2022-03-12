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
#include "configServerBlock.hpp"


class configLocationBlock : public configDefaults
{
    public:
        configLocationBlock()
        {};

        ~configLocationBlock()
        {};

		void	locationDirectiveTreatment(std::string line)
		{
			/***********************************************************/
			/* Ignore the first word add the second argument as the    */
			/* location_path.										   */
			/* Check for more than 2 arguments, if there is more,      */
			/* throw.  												   */
			/* (not a throw).										   */
			/***********************************************************/
			std::stringstream	is(line);
			std::string			word;
			int					number_of_words = 0;
			while (is >> word)
			{
				if (number_of_words == 0)
				{
					number_of_words++;
					continue ;
				}
				if (number_of_words > 2)
				{
					throw std::runtime_error("configLocationBlock.hpp exception: location path has too arguments");
				}
				_location_path.push_back(word);
				number_of_words++;
			}
			if (number_of_words <= 2)
			{
				throw std::runtime_error("configLocationBlock.hpp exception: location path is empty");
			}
		}
		void	errorpageDirectiveTreatment(std::string line)
		{
			/*****************************************************************/
			/* First count how many args.     								 */
			/* Check if it has more than 1 args. 							 */
			/* Check if it's empty.											 */
			/*****************************************************************/
			std::stringstream	is(line);
			std::string			word;
			std::string			path;
			int					number_of_words = 0;
			while (is >> word)
			{
				path = word;
				number_of_words++;
			}

			if (number_of_words == 2 && !word.compare(";"))
			{
				throw std::runtime_error("configServerBlock.hpp exception: error_page has no arguments");
			}

			if (number_of_words < 3)
			{
				throw std::runtime_error("configServerBlock.hpp exception: error_page has wrong number of args");
			}

			/*****************************************************************/
			/* Remove ';' from path.										 */
			/* Add root to the path.										 */
			/* Check if values are between 300 and 599.						 */
			/* Add everything to error status, error path.					 */
			/*****************************************************************/
			path.resize(path.size() - 1);
			_error_path.clear();

			if (_root.empty())
			{
				_error_path.push_back(*_root_default.begin() + path);
			}
			else
			{
				_error_path.push_back(*_root.begin() + path);
			}

			std::stringstream	is2(line);
			int					i = 0;
			_error_status.clear();
			while (is2 >> word)
			{
				if (i  != number_of_words - 1 && i != 0)
				{
					if ((word.find_first_not_of("0123456789") == std::string::npos) == false)
					{
						throw std::runtime_error("configServerBlock.hpp exception: error_page has invalid status code");
					}
					else
					{
						int status_code = atoi(word.c_str());
						if (status_code < 300 || status_code > 599)
						{
							throw std::runtime_error("configServerBlock.hpp exception: error_page has invalid range status code");
						}
						else
						{
							if (std::find(_error_status.begin(), _error_status.end(), status_code) != _error_status.end())
							{}
							else
								_error_status.push_back(status_code);
						}
					}
				}
				i++;
			}
		}
		void	clientmaxbodysizeDirectiveTreatment(std::string line)
		{
			/***********************************************************/
			/* Ignore the first word and treat the second word.		   */
			/* Check if it has more than 1 argument and throw if it    */
			/* has.													   */
			/* Check for duplicates so it doesn't add duplicates 	   */
			/* (throw).												   */
			/* Check for ';' if it's one argument only.				   */
			/* Check for ';' at the end and remove it.				   */
			/* Check for last letter if it's a k K g G m M			   */
			/* Do the conversions.									   */
			/***********************************************************/
			std::stringstream	is(line);
			std::string			word;
			char				size_type;
			long long int		client_max_body_size = 0;
			int					number_of_words = 0;
			while (is >> word)
			{
				if (number_of_words == 0)
				{
					number_of_words++;
					continue ;
				}
				if (number_of_words == 1 && !word.compare(";"))
				{
					throw std::runtime_error("configServerBlock.hpp exception: client_max_body_size has no arguments");
				}
				if (number_of_words > 1)
				{
					throw std::runtime_error("configServerBlock.hpp exception: client_max_body_size has too many arguments");
				}
				if (*word.rbegin() == ';')
				{
					word.resize(word.size() - 1);
				}
				if (*word.rbegin() == 'k' || *word.rbegin() == 'K' ||
					*word.rbegin() == 'g' || *word.rbegin() == 'G' ||
					*word.rbegin() == 'm' || *word.rbegin() == 'M')
				{
					size_type = *word.rbegin();
					word.resize(word.size() - 1);
				}
				if ((word.find_first_not_of("0123456789") == std::string::npos) == false)
				{
					throw std::runtime_error("configServerBlock.hpp exception: client_max_body_size has invalid argument");
				}
				if (size_type == 'K' || size_type == 'k')
					client_max_body_size = atoll(word.c_str()) * 1024;
				else if (size_type == 'M' || size_type == 'm')
					client_max_body_size = atoll(word.c_str()) * (1024 * 1024);
				else if (size_type == 'G' || size_type == 'g')
				{
					client_max_body_size = atoll(word.c_str()) * (1024 * 1024 * 1024);
				}
				else
					client_max_body_size = atoll(word.c_str());
				if (_client_max_body_size.empty())
				{
					_client_max_body_size.push_back(client_max_body_size);
				}
				else
				{
					throw std::runtime_error("configServerBlock.hpp exception: client_max_body_size has duplicated value");
				}
				number_of_words++;
			}
		}
		void	rootDirectiveTreatment(std::string line)
		{
			/***********************************************************/
			/* Ignore the first word and add the rest of the arguments */
			/* to the _root vector.									   */
			/* Check for duplicates so it doesn't add duplicates 	   */
			/* (throw).												   */
			/* Check for ';' if it's one argument only.				   */
			/* Check for ';' at the end and remove it.				   */
			/***********************************************************/
			std::stringstream	is(line);
			std::string			word;
			int					number_of_words = 0;
			while (is >> word)
			{
				if (number_of_words == 0)
				{
					number_of_words++;
					continue ;
				}
				if (number_of_words == 1 && !word.compare(";"))
				{
					throw std::runtime_error("configServerBlock.hpp exception: root has no arguments");
				}
				if (number_of_words > 1)
				{
					throw std::runtime_error("configServerBlock.hpp exception: root has too many arguments");
				}
				if (*word.rbegin() == ';')
				{
					word.resize(word.size() - 1);
				}
				if (_root.empty())
				{
					_root.push_back(word);
				}
				else
				{
					throw std::runtime_error("configServerBlock.hpp exception: root has duplicated value");
				}
				number_of_words++;
			}
		}
		void	indexDirectiveTreatment(std::string line)
		{
			/***********************************************************/
			/* Ignore the first word and add the rest of the arguments */
			/* to the _index vector.	 							   */
			/* Check for duplicates so it doesn't add duplicates 	   */
			/* (not a throw).										   */
			/* Check for ';' if it's one argument only.				   */
			/* Check for ';' at the end and remove it.				   */
			/***********************************************************/
			std::stringstream	is(line);
			std::string			word;
			int					number_of_words = 0;
			while (is >> word)
			{
				if (number_of_words == 0)
				{
					number_of_words++;
					continue ;
				}
				if (number_of_words == 1 && !word.compare(";"))
				{
					throw std::runtime_error("configServerBlock.hpp exception: index has no arguments");
				}
				if (*word.rbegin() == ';')
				{
					word.resize(word.size() - 1);
				}
				if (std::find(_index.begin(), _index.end(), word) != _index.end())
				{}
				else
					_index.push_back(word);
				number_of_words++;
			}
		}
		void	autoindexDirectiveTreatment(std::string line)
		{
			/***********************************************************/
			/* Ignore the first word and treat the second word.		   */
			/* Check if it has more than 1 argument and throw if it    */
			/* has.													   */
			/* Check for duplicates so it doesn't add duplicates 	   */
			/* (throw).												   */
			/* Check for ';' if it's one argument only.				   */
			/* Check for ';' at the end and remove it.				   */
			/***********************************************************/
			std::stringstream	is(line);
			std::string			word;
			int					number_of_words = 0;
			while (is >> word)
			{
				if (number_of_words == 0)
				{
					number_of_words++;
					continue ;
				}
				if (number_of_words == 1 && !word.compare(";"))
				{
					throw std::runtime_error("configServerBlock.hpp exception: auto_index has no arguments");
				}
				if (number_of_words > 1)
				{
					throw std::runtime_error("configServerBlock.hpp exception: auto_index has too many arguments");
				}
				if (*word.rbegin() == ';')
				{
					word.resize(word.size() - 1);
				}
				if (word.compare("on") && word.compare("off"))
				{
					throw std::runtime_error("configServerBlock.hpp exception: auto_index has invalid argument");
				}
				if (_auto_index.empty())
				{
					_auto_index.push_back(word);
				}
				else
				{
					throw std::runtime_error("configServerBlock.hpp exception: auto_index has duplicated value");
				}
				number_of_words++;
			}
		}


		std::vector<std::string>	getLocationPath()	{ return (_location_path); }
		std::vector<int>			getErrorStatus() { return (_error_status); }
		std::vector<std::string>	getErrorPath()	{ return (_error_path); }
		std::vector<long long int>	getClientMaxBodySize()	{ return (_client_max_body_size); }
		std::vector<std::string>	getRoot()	{ return (_root); }
		std::vector<std::string>	getIndex()	{ return (_index); }
		std::vector<std::string>	getAutoindex()	{ return (_auto_index); }
		std::vector<std::string>	getMethods()	{ return (_methods); }
		std::vector<std::string>	getRedirect()	{ return (_redirect); }
		std::vector<std::string>	getCgi()	{ return (_cgi); }
		std::vector<std::string>	getUpload()	{ return (_upload); }

    private:
        /**********************/
        /* Current Directives */
        /**********************/

		// location_path
		std::vector<std::string>	_location_path;

		// error_page
        std::vector<int>            _error_status;
        std::vector<std::string>    _error_path;

        std::vector<long long int>  _client_max_body_size;
        std::vector<std::string>    _root;
        std::vector<std::string>    _index;
        std::vector<std::string>    _auto_index;

        std::vector<std::string>    _methods;
        std::vector<std::string>    _redirect;
        std::vector<std::string>    _cgi;
        std::vector<std::string>    _upload;
};

#endif

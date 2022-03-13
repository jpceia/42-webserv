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
				throw std::runtime_error("configLocationBlock.hpp exception: error_page has no arguments");
			}

			if (number_of_words < 3)
			{
				throw std::runtime_error("configLocationBlock.hpp exception: error_page has wrong number of args");
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
						throw std::runtime_error("configLocationBlock.hpp exception: error_page has invalid status code");
					}
					else
					{
						int status_code = atoi(word.c_str());
						if (status_code < 300 || status_code > 599)
						{
							throw std::runtime_error("configLocationBlock.hpp exception: error_page has invalid range status code");
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
			long int			client_max_body_size = 0;
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
					throw std::runtime_error("configLocationBlock.hpp exception: client_max_body_size has no arguments");
				}
				if (number_of_words > 1)
				{
					throw std::runtime_error("configLocationBlock.hpp exception: client_max_body_size has too many arguments");
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
					throw std::runtime_error("configLocationBlock.hpp exception: client_max_body_size has invalid argument");
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
					throw std::runtime_error("configLocationBlock.hpp exception: client_max_body_size has duplicated value");
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
					throw std::runtime_error("configLocationBlock.hpp exception: root has no arguments");
				}
				if (number_of_words > 1)
				{
					throw std::runtime_error("configLocationBlock.hpp exception: root has too many arguments");
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
					throw std::runtime_error("configLocationBlock.hpp exception: root has duplicated value");
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
					throw std::runtime_error("configLocationBlock.hpp exception: index has no arguments");
				}
				if (*word.rbegin() == ';')
				{
					word.resize(word.size() - 1);
				}
				if (std::find(_index.begin(), _index.end(), word) != _index.end())
				{}
				else
				{
					if (!_root.empty())
					{
						_index.push_back(*_root.begin() + "/" + word);
					}
					else
					{
						_index.push_back(*_root_default.begin() + "/" + word);
					}
				}
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
					throw std::runtime_error("configLocationBlock.hpp exception: auto_index has no arguments");
				}
				if (number_of_words > 1)
				{
					throw std::runtime_error("configLocationBlock.hpp exception: auto_index has too many arguments");
				}
				if (*word.rbegin() == ';')
				{
					word.resize(word.size() - 1);
				}
				if (word.compare("on") && word.compare("off"))
				{
					throw std::runtime_error("configLocationBlock.hpp exception: auto_index has invalid argument");
				}
				if (_auto_index.empty())
				{
					_auto_index.push_back(word);
				}
				else
				{
					throw std::runtime_error("configLocationBlock.hpp exception: auto_index has duplicated value");
				}
				number_of_words++;
			}
		}
		void	returnDirectiveTreatment(std::string line)
		{
			/***********************************************************/
			/* Ignore the first word.								   */
			/* Check how many arguments it has.						   */
			/* If it has 1 argument make sure it's “http://” or        */
			/* “https://”.	   										   */
			/* If it has 2 arguments first is a status code.		   */
			/* If status code is a valid status code.				   */
			/* Check for ';' if it's one argument only.				   */
			/* Check for ';' at the end and remove it.				   */
			/***********************************************************/
			std::stringstream	is(line);
			std::string			first_arg;
			std::string			last_arg;
			int					number_of_words = 0;
			while (is >> last_arg)
			{
				if (number_of_words == 0)
				{
					number_of_words++;
					continue ;
				}
				if (number_of_words == 1 && !last_arg.compare(";"))
				{
					throw std::runtime_error("configLocationBlock.hpp exception: return has no arguments");
				}
				if (number_of_words == 1)
					first_arg = last_arg;
				number_of_words++;
			}

			if (number_of_words == 2)
			{
				if (*last_arg.rbegin() == ';')
				{
					last_arg.resize(last_arg.size() - 1);
				}

				if (last_arg.size() > 6)
				{
					std::string first_seven = last_arg.substr(0, 7);
					if (!first_seven.compare("http://"))
					{
						_redirect_path.clear();
						_redirect_path.push_back(last_arg);
						return ;
					}
				}
				if (last_arg.size() > 7)
				{
					std::string first_eight = last_arg.substr(0, 8);
					if (!first_eight.compare("https://"))
					{
						_redirect_path.clear();
						_redirect_path.push_back(last_arg);
						return ;
					}
				}
				throw std::runtime_error("configLocationBlock.hpp exception: return has wrong URL");
			}

			if (*last_arg.rbegin() == ';')
			{
				last_arg.resize(last_arg.size() - 1);
			}
			_redirect_path.clear();
			_redirect_path.push_back(last_arg);

			if ((first_arg.find_first_not_of("0123456789") == std::string::npos) == false)
			{
				throw std::runtime_error("configLocationBlock.hpp exception: return has invalid argument");
			}

			int status_code = atoi(first_arg.c_str());
			if (status_code < 100 || status_code > 511)
			{
				throw std::runtime_error("configLocationBlock.hpp exception: return has invalid range status code");
			}
			else
			{
				_redirect_status.clear();
				_redirect_status.push_back(status_code);
			}
		}
		void	methodsDirectiveTreatment(std::string line)
		{
			/***********************************************************/
			/* Ignore the first word.								   */
			/* Accept only GET, POST, DELETE.						   */
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
					throw std::runtime_error("configLocationBlock.hpp exception: methods has no arguments");
				}
				if (*word.rbegin() == ';')
				{
					word.resize(word.size() - 1);
				}
				if (!word.compare("GET") || !word.compare("get"))
				{
					if (std::find(_methods.begin(), _methods.end(), "GET") != _methods.end())
					{}
					else
						_methods.push_back("GET");
				}
				else if (!word.compare("POST") || !word.compare("post"))
				{
					if (std::find(_methods.begin(), _methods.end(), "POST") != _methods.end())
					{}
					else
						_methods.push_back("POST");
				}
				else if (!word.compare("DELETE") || !word.compare("delete"))
				{
					if (std::find(_methods.begin(), _methods.end(), "DELETE") != _methods.end())
					{}
					else
						_methods.push_back("DELETE");
				}
				else
				{
					throw std::runtime_error("configLocationBlock.hpp exception: methods has wrong argument");
				}
				number_of_words++;
			}

		}
		void	cgiDirectiveTreatment(std::string line)
		{
			/***********************************************************/
			/* Ignore the first word.								   */
			/* Check how many arguments it has.						   */
			/* It has to have 2 arguments.							   */
			/* Check for ';' if it's one argument only.				   */
			/* Check for ';' at the end and remove it.				   */
			/***********************************************************/
			std::stringstream	is(line);
			std::string			first_arg;
			std::string			last_arg;
			int					number_of_words = 0;
			while (is >> last_arg)
			{
				if (number_of_words == 0)
				{
					number_of_words++;
					continue ;
				}
				if (number_of_words == 1 && !last_arg.compare(";"))
				{
					throw std::runtime_error("configLocationBlock.hpp exception: cgi has no arguments");
				}
				if (number_of_words == 1)
					first_arg = last_arg;
				number_of_words++;
			}

			if (number_of_words != 3)
			{
				throw std::runtime_error("configLocationBlock.hpp exception: cgi has wrong number of arguments");
			}

			if (*last_arg.rbegin() == ';')
			{
				last_arg.resize(last_arg.size() - 1);
			}

			if (_cgi.empty())
			{
				_cgi.push_back(first_arg);
				_cgi.push_back(last_arg);
			}
			else
			{
				throw std::runtime_error("configLocationBlock.hpp exception: cgi has duplicated value");
			}

		}
		void	uploadDirectiveTreatment(std::string line)
		{
			/***********************************************************/
			/* Ignore the first word.								   */
			/* Check how many arguments it has.						   */
			/* It has to have 1 arguments.							   */
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
					throw std::runtime_error("configLocationBlock.hpp exception: upload has no arguments");
				}
				number_of_words++;
			}

			if (number_of_words != 2)
			{
				throw std::runtime_error("configLocationBlock.hpp exception: upload has wrong number of arguments");
			}

			if (*word.rbegin() == ';')
			{
				word.resize(word.size() - 1);
			}

			if (_upload.empty())
			{
				if (_root.empty())
				{
					_upload.push_back(*_root_default.begin() + "/" + word);
				}
				else
				{
					_upload.push_back(*_root.begin() + "/" + word);
				}
			}
			else
			{
				throw std::runtime_error("configLocationBlock.hpp exception: upload has duplicated value");
			}

		}

		/*****************/
		/*    Getters    */
		/*****************/
		std::vector<std::string>	getLocationPath()		{ return (_location_path); }
		std::vector<int>			getErrorStatus() 		{ return (_error_status); }
		std::vector<std::string>	getErrorPath()			{ return (_error_path); }
		std::vector<long int>		getClientMaxBodySize()	{ return (_client_max_body_size); }
		std::vector<std::string>	getRoot()				{ return (_root); }
		std::vector<std::string>	getIndex()				{ return (_index); }
		std::vector<std::string>	getAutoindex()			{ return (_auto_index); }
		std::vector<std::string>	getMethods()			{ return (_methods); }
		std::vector<int>			getRedirectStatus()		{ return (_redirect_status); }
		std::vector<std::string>	getRedirectPath()		{ return (_redirect_path); }
		std::vector<std::string>	getCgi()				{ return (_cgi); }
		std::vector<std::string>	getUpload()				{ return (_upload); }

    private:
        /**********************/
        /* Current Directives */
        /**********************/

		// location_path
		std::vector<std::string>	_location_path;

		// error_page
        std::vector<int>            _error_status;
        std::vector<std::string>    _error_path;

        std::vector<long int>		_client_max_body_size;
        std::vector<std::string>    _root;
        std::vector<std::string>    _index;
        std::vector<std::string>    _auto_index;

        std::vector<std::string>    _methods;

		// return (redirect)
		std::vector<int>            _redirect_status;
        std::vector<std::string>    _redirect_path;

        std::vector<std::string>    _cgi;
        std::vector<std::string>    _upload;
};

#endif

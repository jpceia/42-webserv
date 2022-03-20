/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configLocationBlock.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 15:59:55 by tisantos          #+#    #+#             */
/*   Updated: 2022/03/20 02:34:51 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "configLocationBlock.hpp"

/****************/
/* Constructors */
/****************/
configLocationBlock::configLocationBlock()
{}

configLocationBlock::configLocationBlock(const configLocationBlock& rhs)
{
	*this = rhs;
}

configLocationBlock::~configLocationBlock()
{}

/*****************/
/* Copy Operator */
/*****************/
configLocationBlock& configLocationBlock::operator=(const configLocationBlock& rhs)
{
    if (this == &rhs)
    {
    	return *this;
    }
	_location_path = rhs._location_path;
	_error_page = rhs._error_page;
	_client_max_body_size = rhs._client_max_body_size;
	_root = rhs._root;
	_index = rhs._index;
	_auto_index = rhs._auto_index;
	_methods = rhs._methods;
	_redirect_status = rhs._redirect_status;
	_redirect_path = rhs._redirect_path;
	_cgi = rhs._cgi;
	_upload = rhs._upload;

	return *this;
}

/***********/
/* Methods */
/***********/
void	configLocationBlock::locationDirectiveTreatment(std::string line)
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
void	configLocationBlock::errorpageDirectiveTreatment(std::map<int, std::string> error_p,
									std::string line)
{
	/*****************************************************************/
	/* If there is an error_page on server block. Add here.          */
	/*****************************************************************/
	if (!error_p.empty() && _error_page.empty())
		_error_page = error_p;

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

	std::vector<int>				error_status;
	std::vector<std::string>		error_path;

	if (_root.empty())
	{
		error_path.push_back(*_root_default.begin() + path);
	}
	else
	{
		error_path.push_back(*_root.begin() + path);
	}

	std::stringstream	is2(line);
	int					i = 0;
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
					if (std::find(error_status.begin(), error_status.end(), status_code) != error_status.end())
					{}
					else
						error_status.push_back(status_code);
				}
			}
		}
		i++;
	}

	/*****************************************************************/
	/* Now we have the values in the error_status and error_path.	 */
	/* We may have something like: 									 */
	/*	Error_status = "300 301 302 303"							 */
	/*	Error_path	 = "/404.html"									 */
	/* We put everything on _error_page map like so:				 */
	/*	Error_map[0] = {300, "/404".html}							 */
	/*	Error_map[1] = {301, "/404".html}							 */
	/*	Error_map[2] = {302, "/404".html}							 */
	/*	Error_map[3] = {303, "/404".html}							 */
	/*****************************************************************/

	std::vector<int>::iterator it = error_status.begin();
	for (; it != error_status.end(); it++)
	{
		_error_page[*it] = error_path.front();
	}
}
void	configLocationBlock::clientmaxbodysizeDirectiveTreatment(std::string line)
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
	unsigned long int	client_max_body_size = 0;
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
void	configLocationBlock::rootDirectiveTreatment(std::string line)
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
void	configLocationBlock::indexDirectiveTreatment(std::vector<std::string> ind, std::string line)
{
	/*****************************************************************/
	/* If there is an index on server block. Add here. 		         */
	/*****************************************************************/
	if (!ind.empty() && _index.empty())
		_index = ind;

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
				_index.push_back(word);
			}
			else
			{
				if (std::find(_index.begin(), _index.end(), word) != _index.end())
				{}
				else
					_index.push_back(word);
			}
		}
		number_of_words++;
	}
}
void	configLocationBlock::autoindexDirectiveTreatment(std::string line)
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
void	configLocationBlock::returnDirectiveTreatment(std::string line)
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

// convert string to uppercase
std::string toUpper(const std::string& s)
{
	std::string result(s);
	std::transform(result.begin(), result.end(), result.begin(), ::toupper);
	return result;
}

void	configLocationBlock::methodsDirectiveTreatment(std::string line)
{
	/***********************************************************/
	/* Ignore the first word.								   */
	/* Accept only GET, POST, DELETE.						   */
	/* Check for ';' if it's one argument only.				   */
	/* Check for ';' at the end and remove it.				   */
	/***********************************************************/
	std::istringstream	is(line);
	std::string			word;
	int					number_of_words = 0;
	while (is >> word)
	{
		word = toUpper(word);
		if (number_of_words == 0)
		{
			number_of_words++;
			continue ;
		}
		if (number_of_words == 1 && !word.compare(";"))
			throw std::runtime_error("configLocationBlock.hpp exception: methods has no arguments");

		if (*word.rbegin() == ';')
			word.resize(word.size() - 1);

		try
		{
			HTTPMethod method;
			std::stringstream ss(word);
			ss >> method;
			_methods.insert(method);
		}
		catch (std::exception& e) // unknown method
		{
			throw std::runtime_error("configLocationBlock.hpp exception: methods has wrong argument");
		}
		number_of_words++;
	}

}
void	configLocationBlock::cgiDirectiveTreatment(std::string line)
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

	_cgi[first_arg] = last_arg;
}
void	configLocationBlock::uploadDirectiveTreatment(std::string line)
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

void	configLocationBlock::fillDirectivesIfEmpty(
								std::vector<unsigned long int> client_max_body_size,
								std::vector<std::string> root,
								std::vector<std::string> autoindex,
								std::vector<std::string> index)
{
	if (_client_max_body_size.empty())
		_client_max_body_size = client_max_body_size;
	if (_root.empty())
		_root = root;
	if (_index.empty())
		_index = index;
	if (_auto_index.empty())
		_auto_index = autoindex;
	if (_methods.empty())
		_methods = _methods_default;
	if (_upload.empty())
		_upload = _upload_default;
}

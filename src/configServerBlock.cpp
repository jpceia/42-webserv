#include "configServerBlock.hpp"

configServerBlock::configServerBlock()
{}

configServerBlock::configServerBlock(std::vector<std::string> server_block) : _server_block(server_block)
{}

configServerBlock::~configServerBlock()
{}

void    configServerBlock::fillBlocks()
{
	std::vector<std::string>::iterator it(_server_block.begin());
	int		on_server			= 0;
	int		on_location			= 0;
	_location_blocks_count		= 0; // How many location blocks are there.

	for (;it != _server_block.end(); it++)
	{
		std::string	line;
		int i = 0;

		std::istringstream iss(*it);
		while (iss)
		{
			std::string subs;
			iss >> subs;

			if (i == 0 && !subs.compare("server"))
			{
				on_server = 1;
			}
			else if (i == 0 && !subs.compare("location"))
			{
				on_server = 0;
				on_location = 1;

				// In case server block has a root, location has to have the
				// same root in default.
				if (!_root.empty())
					_location_blocks[_location_blocks_count]._root_default = _root;

				_location_blocks_count++;
			}

			line += subs;
			line += " ";

			i++;
		}
		if (line.size() > 1)
			line.resize(line.size() - 2);

		if (!line.compare("}"))
		{
			on_location = 0;
		}

		if (on_server == 1)
		{
			fillServerBlock(line);
		}
		else if (on_location == 1)
		{
			fillLocationBlock(line);
		}
	}

	/**************************************************/
	/* If listen wasn't found, fill it manually.      */
	/* If server_name wasn't found, fill it manually. */
	/* If root wasn't found, fill it manually.        */
	/**************************************************/
	if (_ip.empty() && _port.empty())
	{
		_ip = _ip_default;
		_port = _port_default;
	}
	if (_server_name.empty())
		_server_name = _server_name_default;
	if (_root.empty())
		_root = _root_default;

	/**************************************************/
	/* If there is no locations add a default '/'     */
	/* If there are location blocks, check to see if  */
	/* there is a '/', if there isn't one, add it     */
	/* manually. 									  */
	/**************************************************/
	if (_location_blocks_count == 0)
	{
		_location_blocks[_location_blocks_count].locationDirectiveTreatment("location / {");
		_location_blocks_count++;
	}
	else
	{
		bool	is_default_block_path = false;
		for (int i = 0; i < _location_blocks_count; i++)
		{
			if (_location_blocks[i]._location_path.front() == "/")
				is_default_block_path = true;
		}
		if (is_default_block_path == false)
		{
			for (int i = _location_blocks_count + 1; i > 0; i--)
			{
				_location_blocks[i] = _location_blocks[i - 1];
			}

			configLocationBlock locationTemp;
			locationTemp.locationDirectiveTreatment("location / {");
			_location_blocks[0] = locationTemp;
			_location_blocks_count++;
		}
	}

	/**************************************************/
	/* Fill Server Blocks directives if empty with    */
	/* defaults.									  */
	/* Fill Location blocks directives if empty with  */
	/* Server Blocks information.					  */
	/**************************************************/
	fillDirectivesIfEmpty();
	for (int i = 0; i < _location_blocks_count; i++)
	{
		_location_blocks[i].fillDirectivesIfEmpty(_client_max_body_size,
													_root,
													_auto_index,
													_index);
	}


}

/****************/
/* Print Helper */
/****************/
void	configServerBlock::printDirectives()
{
	/**********************************/
	/*			  listen              */
	/**********************************/
	std::cout << "Listen:			";
	std::vector<std::string>::iterator	ip_it = _ip.begin();
	std::vector<int>::iterator	port_it = _port.begin();
	for (; ip_it != _ip.end(); ip_it++, port_it++)
	{
		std::cout << "  { " << *ip_it << ":" << *port_it << " }" << std::endl;
	}

	/**********************************/
	/*			  server_name         */
	/**********************************/
	std::cout << "Server_name:		";
	std::vector<std::string>::iterator	server_name_it = _server_name.begin();
	std::cout << "  { ";
	for (; server_name_it != _server_name.end(); server_name_it++)
	{
		std::cout <<  *server_name_it << " ";
	}
	std::cout << "}" << std::endl;

	/**********************************/
	/*			  error_page          */
	/**********************************/
	if (!_error_page.empty())
	{
		std::cout << "Error_page:";
		std::map<int, std::string>::iterator	_error_page_it = _error_page.begin();

		for (int i = 0; _error_page_it != _error_page.end(); _error_page_it++, i++)
		{
			if (i == 0)
				std::cout << "		  { ";
			else
				std::cout << "			  { ";
			std::cout <<  _error_page_it->first << " " << _error_page_it->second;
			std::cout << " }" << std::endl;
		}
	}

	/**********************************/
	/*			    root              */
	/**********************************/
	std::cout << "Root:			";
	std::cout << "  { " << *_root.begin() << " }" << std::endl;

	/**********************************/
	/*		 client_max_body_size     */
	/**********************************/
	if (!_client_max_body_size.empty())
	{
		std::cout << "Client_max_body_size:	";
		std::cout << "  { " << *_client_max_body_size.begin() << " }" << std::endl;
	}

	/**********************************/
	/*				index             */
	/**********************************/
	if (!_index.empty())
	{
		std::cout << "Index:			";
		std::vector<std::string>::iterator	index_it = _index.begin();
		std::cout << "  { ";
		for (; index_it != _index.end(); index_it++)
		{
			std::cout <<  *index_it << " ";
		}
		std::cout << "}" << std::endl;
	}

	/**********************************/
	/*			  auto_index          */
	/**********************************/
	if (!_auto_index.empty())
	{
		std::cout << "Auto_index:		";
		std::cout << "  { " << *_auto_index.begin() << " }" << std::endl;
	}

	/**********************************/
	/*			  location            */
	/**********************************/
	for (int i = 0; i < _location_blocks_count; i++)
	{
		std::cout << "Location:		";
		std::cout << "  { " << *_location_blocks[i].getLocationPath().begin() << " }" << std::endl;

		/**********************************/
		/*			  error_page          */
		/**********************************/
		std::map<int, std::string> error_page_l = _location_blocks[i].getErrorPage();
		if (!error_page_l.empty())
		{
			std::cout << "  Error_page:   ";
			std::map<int, std::string>::iterator	_error_page_it = error_page_l.begin();

			for (int i = 0; _error_page_it != error_page_l.end(); _error_page_it++, i++)
			{
				if (i == 0)
					std::cout << "	     { ";
				else
					std::cout << "  			     { ";
				std::cout <<  _error_page_it->first << " " << _error_page_it->second;
				std::cout << " }" << std::endl;
			}
		}

		/**********************************/
		/*			    root              */
		/**********************************/
		if (!_location_blocks[i].getRoot().empty())
		{
			std::cout << "  Root:			   ";
			std::cout << "  { " << *_location_blocks[i].getRoot().begin() << " }" << std::endl;
		}

		/**********************************/
		/*		 client_max_body_size     */
		/**********************************/
		std::vector<unsigned long int> client_max_body_size_l = _location_blocks[i]._client_max_body_size;

		if (!client_max_body_size_l.empty())
		{
			std::cout << "  Client_max_body_size:	   ";
			std::cout << "  { " << *client_max_body_size_l.begin() << " }" << std::endl;
		}

		/**********************************/
		/*				index             */
		/**********************************/
		std::vector<std::string> index_l = _location_blocks[i].getIndex();
		if (!index_l.empty())
		{
			std::cout << "  Index:		   ";
			std::vector<std::string>::iterator	index_it = index_l.begin();
			std::cout << "  { ";
			for (; index_it != index_l.end(); index_it++)
			{
				std::cout <<  *index_it << " ";
			}
			std::cout << "}" << std::endl;
		}

		/**********************************/
		/*			  auto_index          */
		/**********************************/
		if (!_location_blocks[i].getAutoIndex().empty())
		{
			std::cout << "  Auto_index:		   ";
			std::cout << "  { " << *_location_blocks[i].getAutoIndex().begin() << " }" << std::endl;
		}

		/**********************************/
		/*			  return              */
		/**********************************/
		if (!_location_blocks[i]._redirect_path.empty() ||
			!_location_blocks[i]._redirect_status.empty())
		{
			std::cout << "  Return:		   ";
			if (!_location_blocks[i]._redirect_status.empty())
			{
				std::cout << "  { " << *_location_blocks[i]._redirect_status.begin() << " "
									<< *_location_blocks[i]._redirect_path.begin() << " }" << std::endl;
			}
			else if (!_location_blocks[i]._redirect_path.empty())
			{
				std::cout << "  { " << *_location_blocks[i]._redirect_path.begin() << " }" << std::endl;
			}
		}

		/**********************************/
		/*			  methods             */
		/**********************************/
		std::vector<std::string> methods_l = _location_blocks[i].getMethods();
		if (!methods_l.empty())
		{
			std::cout << "  Methods:		   ";
			std::vector<std::string>::iterator	methods_it = methods_l.begin();
			std::cout << "  { ";
			for (; methods_it != methods_l.end(); methods_it++)
			{
				std::cout <<  *methods_it << " ";
			}
			std::cout << "}" << std::endl;
		}

		/**********************************/
		/*			    cgi               */
		/**********************************/
		std::map<std::string, std::string> cgi_l = _location_blocks[i].getCgi();
		if (!cgi_l.empty())
		{
			std::cout << "  Cgi:			   ";
			std::map<std::string, std::string>::iterator	cgi_it = cgi_l.begin();
			for (int i = 0; cgi_it != cgi_l.end(); cgi_it++, i++)
			{
				if (i == 0)
					std::cout << "  { ";
				else
					std::cout << "  			     { ";
				std::cout << cgi_it->first << " " << cgi_it->second << "}" << std::endl;
			}
		}

		/**********************************/
		/*			   upload             */
		/**********************************/
		std::vector<std::string> upload_l = _location_blocks[i]._upload;
		if (!upload_l.empty())
		{
			std::cout << "  Upload:		   ";
			std::vector<std::string>::iterator	upload_it = upload_l.begin();
			std::cout << "  { ";
			for (; upload_it != upload_l.end(); upload_it++)
			{
				std::cout <<  *upload_it << " ";
			}
			std::cout << "}" << std::endl;
		}
	}
}

/********************/
/* Helper Functions */
/********************/
void	configServerBlock::fillServerBlock(std::string line)
{
	std::istringstream	iss(line);
	while (iss)
	{
		std::string subs;
		iss >> subs;

		if (!subs.compare("server"))
		{}
		else if (!subs.compare("listen"))
			listenDirectiveTreatment(line);
		else if (!subs.compare("server_name"))
			servernameDirectiveTreatment(line);
		else if (!subs.compare("error_page"))
			errorpageDirectiveTreatment(line);
		else if (!subs.compare("client_max_body_size"))
			clientmaxbodysizeDirectiveTreatment(line);
		else if (!subs.compare("root"))
			rootDirectiveTreatment(line);
		else if (!subs.compare("index"))
			indexDirectiveTreatment(line);
		else if (!subs.compare("autoindex"))
			autoindexDirectiveTreatment(line);
		else if (!subs.compare("}"))
		{}
		else
		{
			std::cerr << "configServerBlock.hpp exception: " << subs << " not valid on server block";
			throw std::runtime_error("");
		}
		return ;
	}
}
void	configServerBlock::fillLocationBlock(std::string line)
{
	std::istringstream	iss(line);
	while (iss)
	{
		std::string subs;
		iss >> subs;

		if (!subs.compare("location"))
			_location_blocks[_location_blocks_count - 1].locationDirectiveTreatment(line);
		else if (!subs.compare("error_page"))
			_location_blocks[_location_blocks_count - 1].errorpageDirectiveTreatment(_error_page, line);
		else if (!subs.compare("client_max_body_size"))
			_location_blocks[_location_blocks_count - 1].clientmaxbodysizeDirectiveTreatment(line);
		else if (!subs.compare("root"))
			_location_blocks[_location_blocks_count - 1].rootDirectiveTreatment(line);
		else if (!subs.compare("index"))
			_location_blocks[_location_blocks_count - 1].indexDirectiveTreatment(_index, line);
		else if (!subs.compare("autoindex"))
			_location_blocks[_location_blocks_count - 1].autoindexDirectiveTreatment(line);
		else if (!subs.compare("return"))
			_location_blocks[_location_blocks_count - 1].returnDirectiveTreatment(line);
		else if (!subs.compare("methods"))
			_location_blocks[_location_blocks_count - 1].methodsDirectiveTreatment(line);
		else if (!subs.compare("cgi"))
			_location_blocks[_location_blocks_count - 1].cgiDirectiveTreatment(line);
		else if (!subs.compare("upload"))
			_location_blocks[_location_blocks_count - 1].uploadDirectiveTreatment(line);
		else if (!subs.compare("}"))
		{}
		else
		{
			std::cerr << "configServerBlock.hpp exception: " << subs << " not valid on location block";
			throw std::runtime_error("");
		}
		checkDuplicatedLocation();
		return ;
	}
}
void	configServerBlock::checkDuplicatedLocation()
{
	for (int i = 0; i < _location_blocks_count; i++)
	{
		for (int x = 0; x < _location_blocks_count; x++)
		{
			if (i != x)
			{
				if (_location_blocks[i].getLocationPath() == _location_blocks[x].getLocationPath())
				{
					throw std::runtime_error("configServerBlock.hpp exception: location has duplicated value");
				}
			}
		}
	}
}

std::vector<std::string>	configServerBlock::returnSplitedDelimiter(std::string line,
																	  std::string delimiter)
{
	size_t pos = 0;
	std::string path_temp = line;
	std::vector<std::string> path_vector;
	while ((pos = path_temp.find(delimiter)) != std::string::npos) {
		path_vector.push_back(path_temp.substr(0, pos));
		path_temp.erase(0, pos + delimiter.length());
	}
	path_vector.push_back(path_temp);
	return (path_vector);
}

/************************/
/* Directives Treatment */
/************************/

void	configServerBlock::listenDirectiveTreatment(std::string line)
{
	/*****************************************************************/
	/* First count how many args.     								 */
	/* Extract the first argument if it has.						 */
	/* listen can only have 1 arg which is the IP and PORT together. */
	/*****************************************************************/
	std::stringstream	is(line);
	std::string			word;
	std::string			argument;
	int					number_of_words = 0;
	while (is >> word)
	{
		if (number_of_words == 1)
			argument = word;
		number_of_words++;
	}

	if (number_of_words != 2)
	{
		throw std::runtime_error("configServerBlock.hpp exception: listen has wrong number of args");
	}

	/*****************************************************************/
	/* Argument was extracted previously.							 */
	/* First check if it's just a ;									 */
	/* Then remove the ending ';'									 */
	/* If it has a : divide it in two.								 */
	/* If it doesn't have a : define if it's a PORT or IP.			 */
	/* IP can be "localhost" or an IP like (0.0.0.0).				 */
	/* PORT can only be an integer.									 */
	/*****************************************************************/
	if (!argument.compare(";"))
	{
		throw std::runtime_error("configServerBlock.hpp exception: listen has no arguments");
	}

	argument.resize(argument.size() - 1);

	std::string	ip_extracted;
	std::string	port_extracted;

	int pos = argument.find_last_of(':');
	if (pos > 0)
	{
		ip_extracted = argument.substr(0, pos);
		port_extracted = argument.substr(pos+1);

		if ((port_extracted.find_first_not_of("0123456789") == std::string::npos) == false)
		{
			throw std::runtime_error("configServerBlock.hpp exception: listen has invalid PORT argument");
		}

		if (ip_extracted.find_first_of('.') == 0)
		{
			if (ip_extracted.compare("localhost"))
			{
				throw std::runtime_error("configServerBlock.hpp exception: listen has invalid IP argument");
			}
		}
		else
		{
			_ip.push_back(ip_extracted);
			_port.push_back(atoi(port_extracted.c_str()));
			listenCheckForDuplicates();
			return ;
		}
		throw std::runtime_error("configServerBlock.hpp exception: listen has invalid IP argument");
	}

	if (argument.find_first_of('.') == 0)
	{
		if (!argument.compare("localhost"))
		{
			_ip.push_back("localhost");
			_port.push_back(80);
			listenCheckForDuplicates();
			return ;
		}
		else if ((argument.find_first_not_of("0123456789") == std::string::npos) == false)
		{
			throw std::runtime_error("configServerBlock.hpp exception: listen has invalid IP or PORT argument");
		}
		else
		{
			_ip.push_back("0.0.0.0");
			_port.push_back(atoi(argument.c_str()));
			listenCheckForDuplicates();
		}
	}
	else
	{
		if ((argument.find_first_not_of("0123456789") == std::string::npos) == false)
		{
			_ip.push_back(argument);
			_port.push_back(80);
			listenCheckForDuplicates();
			return ;
		}
		else
		{
			_ip.push_back("0.0.0.0");
			_port.push_back(atoi(argument.c_str()));
			listenCheckForDuplicates();
		}
	}
}
void	configServerBlock::listenCheckForDuplicates()
{
	std::vector<std::string>::iterator	ip_it = _ip.begin();
	std::vector<int>::iterator	port_it = _port.begin();
	for (int i = 0; ip_it != _ip.end(); ip_it++, port_it++)
	{
		std::vector<std::string>::iterator	ip_compare = _ip.begin();
		std::vector<int>::iterator	port_compare = _port.begin();
		for (int x = 0; ip_compare != _ip.end(); ip_compare++, port_compare++)
		{
			if (i != x)
			{
				if (!ip_it->compare(*ip_compare) && *port_it == *port_compare)
					throw std::runtime_error("configServerBlock.hpp exception: listen has duplicated value");
			}
			x++;
		}
		i++;
	}
}

void	configServerBlock::servernameDirectiveTreatment(std::string line)
{
	/***********************************************************/
	/* Ignore the first word and add the rest of the arguments */
	/* to the _server_name vector. 							   */
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
			throw std::runtime_error("configServerBlock.hpp exception: server_name has no arguments");
		}
		if (*word.rbegin() == ';')
		{
			word.resize(word.size() - 1);
		}
		if (std::find(_server_name.begin(), _server_name.end(), word) != _server_name.end())
		{}
		else
			_server_name.push_back(word);
		number_of_words++;
	}
}
void	configServerBlock::errorpageDirectiveTreatment(std::string line)
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
void	configServerBlock::rootDirectiveTreatment(std::string line)
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
void	configServerBlock::clientmaxbodysizeDirectiveTreatment(std::string line)
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
	std::stringstream		is(line);
	std::string				word;
	char					size_type;
	unsigned long int		client_max_body_size = 0;
	int						number_of_words = 0;
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
void	configServerBlock::indexDirectiveTreatment(std::string line)
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
void	configServerBlock::autoindexDirectiveTreatment(std::string line)
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

void	configServerBlock::fillDirectivesIfEmpty()
{
	if (_server_name.empty())
		_server_name = _server_name_default;
	if (_client_max_body_size.empty())
		_client_max_body_size = _client_max_body_size_default;
	if (_root.empty())
		_root = _root_default;
	if (_index.empty())
	{
		_index.push_back(_root.front() + _index_default.front());
	}
	if (_auto_index.empty())
		_auto_index = _auto_index_default;

}

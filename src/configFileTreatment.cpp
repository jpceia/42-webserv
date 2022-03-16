#include "configFileTreatment.hpp"

configFileTreatment::configFileTreatment()
{}

configFileTreatment::configFileTreatment(std::string configuration_file_raw)
										:	_configuration_file_raw(configuration_file_raw)
{}

configFileTreatment::~configFileTreatment()
{}

void	configFileTreatment::treatConfigurationFile()
{
	treatConfigurationFile_StepOne();
	treatConfigurationFile_StepTwo();
	treatConfigurationFile_StepThree();
	treatConfigurationFile_StepFour();
}

void	configFileTreatment::separateToServerBlocks()
{
	std::vector<std::string>    temp_server_block;

	std::vector<std::string>::iterator it = _configuration_file_treated.begin();
	temp_server_block.push_back(*it);
	it++;

	for (; it != _configuration_file_treated.end(); it++)
	{
		if (*it == "server {")
		{
			_server_blocks.push_back(temp_server_block);
			temp_server_block.clear();
			temp_server_block.push_back(*it);
			continue ;
		}
		temp_server_block.push_back(*it);
	}
	_server_blocks.push_back(temp_server_block);

}

void	configFileTreatment::printConfigurationFileTreated()
{
	std::vector<std::string>::iterator it(_configuration_file_treated.begin());
	for (int i = 0; it != _configuration_file_treated.end(); it++)
	{
		std::cout << i  << " = " << *it << std::endl;
		i++;
	}
}

void	configFileTreatment::printServerBlocks()
{
	std::list<std::vector<std::string> >::iterator it(_server_blocks.begin());

	for (int i = 0; it != _server_blocks.end(); it++)
	{
		std::cout << "-----Server Block [" << i << "]-----" << std::endl;
		std::vector<std::string> vect = *it;
		std::vector<std::string>::iterator it_vect(vect.begin());
		for (int x = 0; it_vect != vect.end(); it_vect++)
		{
			std::cout << x  << " = " << *it_vect << std::endl;
			x++;
		}
		i++;
	}
}

std::list<std::vector<std::string> >	configFileTreatment::getServerBlocks()
{
	return _server_blocks;
}

/********************/
/* Helper Functions */
/********************/
void	configFileTreatment::treatConfigurationFile_StepOne()
{
	/*******************************************************************/
	/* First step:                									   */
	/*   Put on _configuration_file_treated each line until    	       */
	/*   linebreak the _configuration_file_raw. 					   */
	/*																   */
	/*   The Vector result will be:									   */
	/*																   */
	/*     line 0  = server											   */
	/*     line 1  = {												   */
	/*     line 2  =		listen 127.0.0.1:8000;					   */
	/*     line 3  =			server_name localhost;  			   */
	/*     line 4  =		root www/html;	methods GET;			   */
	/*     line 5  =    location / {								   */
	/*     line 6  =       index index.html;						   */
	/*     line 7  =    }											   */
	/*     line 8  = }												   */
	/*     line 9  = server	#hello									   */
	/*     line 10 = {												   */
	/*     line 11 =		listen 127.0.0.1:90;					   */
	/*     line 12 = 		server_name _;							   */
	/*     line 13 = 	#	location								   */
	/*     line 14 = 												   */
	/*     line 15 = }												   */
	/*******************************************************************/
	std::string								 result;
	std::vector<std::string>                 temp_treated;

	std::istringstream iss(_configuration_file_raw);

	for (std::string line; std::getline(iss, line, '\n');)
	{
		result = line;
		temp_treated.push_back(result);
	}
	_configuration_file_treated = temp_treated;
	temp_treated.clear();

	// printConfigurationFileTreated(_configuration_file_treated);
}
void	configFileTreatment::treatConfigurationFile_StepTwo()
{
	/*******************************************************************/
	/* Second step:                									   */
	/*   Remove all # comments, unnecessary tabs and spaces.		   */
	/*   Seperate several ; from the same line.						   */
	/*																   */
	/*     line 0   = server										   */
	/*     line 1   = {												   */
	/*     line 2   = listen 127.0.0.1:8000;						   */
	/*     line 3   = server_name localhost;  				  		   */
	/*     line 4   = root www/html;								   */
	/*	   line 5   = methods GET;									   */
	/*     line 6   = location / {									   */
	/*     line 7   = index index.html;								   */
	/*     line 8   = }												   */
	/*     line 9   = }												   */
	/*     line 10  = server									 	   */
	/*     line 11  = {												   */
	/*     line 12  = listen 127.0.0.1:90;							   */
	/*     line 13  = server_name _;								   */
	/*     line 14  = }												   */
	/*******************************************************************/

	std::vector<std::string>    temp_configuration_file_treated;

	std::vector<std::string>::iterator it = _configuration_file_treated.begin();
	for (; it != _configuration_file_treated.end(); it++)
	{
		std::string	treated_string;

		std::istringstream iss(*it);
		while (iss)
		{
			std::string subs;
			iss >> subs;

			if (subs.compare("#") == 0 || subs[0] == '#')
			{
				treated_string += " ";
				break ;
			}
			if (*subs.rbegin() == ';')
			{
				treated_string += subs;
				temp_configuration_file_treated.push_back(treated_string);
				treated_string.clear();
				continue ;
			}
			treated_string += subs;
			treated_string += " ";
		}
		if (treated_string.size() > 1)
			treated_string.resize(treated_string.size() - 2);
		else
			treated_string.resize(treated_string.size() - 1);

		if (treated_string.size() != 0)
			temp_configuration_file_treated.push_back(treated_string);
	}

	_configuration_file_treated = temp_configuration_file_treated;

	//printConfigurationFileTreated(_configuration_file_treated);
}
void	configFileTreatment::treatConfigurationFile_StepThree()
{
	/*******************************************************************/
	/* Third step:                									   */
	/*   Put everything back in a single line string.				   */
	/*   Format it so that each new line is after a {, a ; or a }	   */
	/*																   */
	/*     line 0   = server {										   */
	/*     line 1   = listen 127.0.0.1:8000;						   */
	/*     line 2   = server_name localhost;  				  		   */
	/*     line 3   = root www/html;								   */
	/*	   line 4   = methods GET;									   */
	/*     line 5   = location / {									   */
	/*     line 6   = index index.html;								   */
	/*     line 7   = }												   */
	/*     line 8   = }												   */
	/*     line 9   = server {										   */
	/*     line 10  = listen 127.0.0.1:90;							   */
	/*     line 11  = server_name _;								   */
	/*     line 12  = }												   */
	/*******************************************************************/
	std::vector<std::string>    temp_configuration_file_treated;
	std::vector<std::string>::iterator it = _configuration_file_treated.begin();
	std::string	temp_treated_string;

	for (; it != _configuration_file_treated.end(); it++)
	{
		std::istringstream iss(*it);
		while (iss)
		{
			std::string subs;
			iss >> subs;

			temp_treated_string += subs;
			temp_treated_string += " ";
		}
		temp_treated_string.resize(temp_treated_string.size() - 1);
	}
	temp_treated_string.resize(temp_treated_string.size() - 1);

	std::istringstream temp_iss(temp_treated_string);
	std::string	temp_string;
	while (temp_iss)
	{
		std::string subs;
		temp_iss >> subs;

		if (*subs.rbegin() == '{' ||
			*subs.rbegin() == '}' ||
			*subs.rbegin() == ';')
		{
			temp_string += subs;
			temp_configuration_file_treated.push_back(temp_string);
			temp_string.clear();
		}
		else
		{
			temp_string += subs;
			temp_string += " ";
		}
	}
	_configuration_file_treated = temp_configuration_file_treated;
	//printConfigurationFileTreated(_configuration_file_treated);
}
void	configFileTreatment::treatConfigurationFile_StepFour()
{
	/*******************************************************************/
	/* Fourth step:                									   */
	/*   Put everything on a single line string.					   */
	/*   Check if Server blocks have opening and closing brackets,	   */
	/*   if locations have opening and closing brackets and if they    */
	/*   are inside server blocks.									   */
	/*******************************************************************/
	std::vector<std::string>::iterator it = _configuration_file_treated.begin();
	std::string	temp_treated_string;

	for (; it != _configuration_file_treated.end(); it++)
	{
		std::istringstream iss(*it);
		while (iss)
		{
			std::string subs;
			iss >> subs;

			temp_treated_string += subs;
			temp_treated_string += " ";
		}
		temp_treated_string.resize(temp_treated_string.size() - 1);
	}
	temp_treated_string.resize(temp_treated_string.size() - 1);

	std::istringstream temp_iss(temp_treated_string);
	int inside_server = 0, inside_location = 0,
		server_bracket_count = 0, location_bracket_count = 0;
	while (temp_iss)
	{
		std::string subs;
		temp_iss >> subs;

		if (!subs.compare("server"))
			inside_server++;
		else if (!subs.compare("location"))
			inside_location++;

		if (!subs.compare("{") && inside_location == 0)
			server_bracket_count++;
		else if (!subs.compare("{") && inside_location > 0)
			location_bracket_count++;

		if (!subs.compare("}") && inside_location > 0)
		{
			inside_location--;
			location_bracket_count--;
		}
		else if (!subs.compare("}") && inside_location == 0)
		{
			inside_server--;
			server_bracket_count--;
		}

		if (inside_location == 1 && inside_server == 0)
		{
			throw std::runtime_error("configFileTreatment.hpp exception: location block outside server block");
		}
	}
	if (server_bracket_count != 0 || inside_server == 1)
	{
		throw std::runtime_error("configFileTreatment.hpp exception: server block brackets missing or too many");
	}
	if (location_bracket_count != 0 || inside_location == 1)
	{
		throw std::runtime_error("configFileTreatment.hpp exception: location block brackets missing or too many");
	}
}

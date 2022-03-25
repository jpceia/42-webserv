/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 16:00:08 by tisantos          #+#    #+#             */
/*   Updated: 2022/03/19 16:28:45 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "debug.hpp"

void debug_context(const struct Context& ctx)
{
	std::cout << "----------------DEBUG CONTEXT----------------" << std::endl;

	std::cout << "PATH = " << ctx.path << std::endl;
	for (std::map<int, std::string>::const_iterator it = ctx.error_page.begin();
		 it != ctx.error_page.end(); it++)
	{
		std::cout << "ERROR_PAGE = " << it->first << " " << it->second << std::endl;
	}
    std::cout << "MAX_BODY_SIZE = " << ctx.max_body_size << std::endl;
    std::cout << "ROOT = " << ctx.root << std::endl;
	std::cout << "REL_PATH = " << ctx.rel_path << std::endl;
	std::cout << "BASE_PATH = " << ctx.base_path << std::endl;
	std::cout << "AUTO_INDEX = " << ctx.autoindex << std::endl;
	for (std::vector<std::string>::const_iterator it = ctx.index.begin();
		 it != ctx.index.end(); it++)
	{
		std::cout << "INDEX = " << *it << std::endl;
	}
	std::cout << "REDIRECT_STATUS = " << ctx.redirect_status << std::endl;
	std::cout << "REDIRECT_PATH = " << ctx.path << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = ctx.cgi.begin();
		 it != ctx.cgi.end(); it++)
	{
		std::cout << "CGI = " << it->first << " " << it->second << std::endl;
	}
	std::cout << "UPLOAD_PATH = " << ctx.upload_path << std::endl;
	std::cout << "SERVER_NAME = " << ctx.server_name << std::endl;
	std::cout << "SERVER_ADDR = " << ctx.server_addr << std::endl;
	std::cout << "CLIENT_ADDR = " << ctx.client_addr << std::endl;
	std::cout << "SERVER_PORT = " << ctx.server_port << std::endl;
	std::cout << "CLIENT_PORT = " << ctx.client_port << std::endl;
	std::cout << "---------------------------------------------" << std::endl;
    std::cout << std::endl;
}

void debug_request(const HTTPRequest& request)
{
	std::cout << "----------------DEBUG REQUEST----------------" << std::endl;

	std::cout << "METHODS = " << request.getMethod() << std::endl;
	std::cout << "PATH = " << request.getPath() << std::endl;
	std::cout << "VERSION = " << request.getVersion() << std::endl;

	std::map<std::string, std::string> headers = request.getHeaders();
	std::map<std::string, std::string>::const_iterator it = headers.begin();
	for (; it != headers.end(); it++)
	{
		std::cout << "HEADERS = " << it->first << " " << it->second << std::endl;
	}
	std::cout << "QUERY_STRING = " << request.getQueryString() << std::endl;
	std::cout << "END_POINT = " << request.getEndpoint() << std::endl;
	std::cout << "BODY = " << request.getBody() << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    std::cout << std::endl;
}

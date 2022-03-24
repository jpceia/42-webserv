/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 17:30:40 by jceia             #+#    #+#             */
/*   Updated: 2022/03/24 07:29:06 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "TCPServer.hpp"
# include "HTTPServer.hpp"
# include "HTTPRequest.hpp"
# include "HTTPResponse.hpp"
# include "HTTPConnection.hpp"
# include "HTTPListener.hpp"
# include "HTTPStatefulConnection.hpp"
# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>
# include <algorithm>
# include <sys/poll.h>
# include <dirent.h>
# include <cstring>
# include "utils.hpp"


HTTPServer::HTTPServer(configFile config, int timeout) :
    TCPServer(timeout),
    _config(config)
{
}

HTTPServer::HTTPServer(const HTTPServer& rhs) :
    TCPServer(),
    _config(rhs._config)
{
}

HTTPServer::~HTTPServer()
{
}

HTTPServer& HTTPServer::operator=(const HTTPServer &rhs)
{
    (void)rhs;
    return *this;
}

void HTTPServer::init()
{
    std::set<std::pair<std::string, int> > ips_and_ports;
    std::vector<configServerBlock> server_blocks = _config.getServerBlocksObj();
    for (std::vector<configServerBlock>::iterator it = server_blocks.begin();
        it != server_blocks.end(); ++it)
        ips_and_ports.insert(std::make_pair(it->getIP(), it->getPort()));
    for (std::set<std::pair<std::string, int> >::const_iterator it = ips_and_ports.begin();
        it != ips_and_ports.end(); ++it)
    {
        // filter for ip and port
        std::vector<configServerBlock> serverBlocks;
        for (std::vector<configServerBlock>::iterator it2 = server_blocks.begin();
            it2 != server_blocks.end(); ++it2)
            if (it->first == it2->getIP() && it->second == it2->getPort())
                serverBlocks.push_back(*it2);

        this->add_listener(new HTTPListener(it->first, it->second, serverBlocks));
    }
}

void HTTPServer::_handle_client_recv(TCPConnection* connection, short& event)
{
    HTTPStatefulConnection* conn = dynamic_cast<HTTPStatefulConnection*>(connection);
    if (conn == NULL)
        throw std::runtime_error("HTTPServer::_handle_client_request: dynamic_cast failed");
    
    bool finished;
    try
    {
        finished = conn->recvChunk();
    }
    catch (const HTTPMessage::ParseException& err) // Illegal HTTP request
    {
        conn->setResponse(_status_page_response(400, _get_context(conn)));
        event = POLLOUT;
        return ;
    }
    
    if (finished) // Finished receiving the request
    {
        HTTPRequest request = conn->getRequest();

        #ifdef DEBUG
        std::cout << "Request: " << std::endl;
        request.print();
        #endif

        // Construct the context
        Context ctx = _get_context(conn);

        // build the response
        HTTPResponse response = _response(conn->getRequest(), ctx);

        if (request.getHeader("Connection") == "keep-alive")
            response.setHeader("Connection", "keep-alive");

        #ifdef DEBUG
        std::cout << "Response: " << std::endl;
        response.print();
        #endif

        conn->setResponse(response);
        event = POLLOUT;
    }
    else
        event = POLLIN;
}

void HTTPServer::_handle_client_send(TCPConnection* connection, short& event)
{
    HTTPStatefulConnection* conn = dynamic_cast<HTTPStatefulConnection*>(connection);
    if (conn == NULL)
        throw std::runtime_error("HTTPServer::_handle_client_request: dynamic_cast failed");

    bool finished = conn->sendChuck();
    if (finished)
    {
        if (conn->getRequest().getHeader("Connection") == "close") // close connection
        {
            #ifdef DEBUG
            std::cout << "Connection: close" << std::endl;
            #endif
            _close_connection(connection);
        }
        else
        {
            conn->clear();  // Clear the buffers
            event = POLLIN;
        }
    }
    else
        event = POLLOUT;
}

HTTPResponse HTTPServer::_response(const HTTPRequest& request, Context& ctx)
{
    // checking if the method is allowed
    if (ctx.allowed_methods.find(request.getMethod()) == ctx.allowed_methods.end())
        return _status_page_response(405, ctx);

    // checking the body size
    if (request.getBody().size() > (size_t)ctx.max_body_size)
        return _status_page_response(413, ctx);

    // checking if it is a redirection
    if (!ctx.redirect_path.empty())
        return _redirect_response(ctx);

    if (!ctx.upload_path.empty())
        return _upload_response(request, ctx);

    // checking if the file exists
    std::string path = ctx.getSysPath();
    if (is_dir(path))
    {
        // std::cout << "path is a directory" << std::endl;
        bool found = false;
        for (std::vector<std::string>::const_iterator it = ctx.index.begin();
            it != ctx.index.end(); ++it)
        {
            // std::cout << "path: " << ctx.base_path << "\t index:" << *it << std::endl;
            if (is_readable_file(path + "/" + *it))
            {
                ctx.sys_rel_path = cleanup_path(ctx.sys_rel_path + "/" + *it);
                found = true;
                break ;
            }
        }
        if (!found)
        {
            if (ctx.autoindex == "on")
                return _autoindex_response(ctx);
            else
                return _status_page_response(404, ctx);
        }
    }
    else if (!is_readable_file(path))
        return _status_page_response(404, ctx);

    // CGI
    map_str_str::const_iterator it = ctx.cgi_bin.find(_get_file_extension(ctx.sys_rel_path));
    if (it != ctx.cgi_bin.end())
        return _cgi_response(it->second, request, ctx);

    // static file
    return _static_response(ctx);
}

HTTPResponse HTTPServer::_static_response(const Context& ctx) const
{
    // Static page
    std::ifstream ifs(ctx.getSysPath().c_str(), std::ifstream::in);
    if (!ifs.good())
    {
        // cannot open file
        return _status_page_response(500, ctx);
    }

    // std::cout << "path is " << ctx.path << std::endl;

    HTTPResponse response;
    response.setHeader("Content-Type", "text/html");
    response.setBody(ifs);
    ifs.close();
    return response;
}

HTTPResponse HTTPServer::_cgi_response(const std::string& cmd, const HTTPRequest& request, const Context& ctx)
{
    std::vector<std::string> args;
    args.push_back(cmd);
    //args.push_back(ctx.path);

    map_str_str env = _get_cgi_env(request, ctx);
    std::stringstream ss(exec_cmd(cmd, args, env, request.getBody()));
    HTTPResponse response;
    ss >> dynamic_cast<HTTPMessage&>(response);

    // https://www.php.net/manual/en/function.http-response-code.php
    // Get the response code from the header
    std::string status = response.getHeader("Status");
    if (!status.empty())
    {
        size_t pos = status.find(' ');
        if (pos != std::string::npos)
        {
            std::string code = status.substr(0, pos);
            response.setStatus(ft_stoi(code)); // Ignore the message
        }
        response.removeHeader("Status");
    }
    return response;
}

// Client_port or Server_port is wrong
// Need to change it and then change it here:
// ss << "\t\t<p><a href=\"http://" + ctx.client_addr + ":" <<\
// "8080" << dirName + "/" + dirEntry + "\">" + dirEntry + "</a></p>\n";
HTTPResponse HTTPServer::_autoindex_response(const Context& ctx) const
{
	DIR *dpdf;
	struct dirent *epdf = NULL;

	std::vector<std::string> directories; //    /www/blog ,       /www/here/blabla
	std::vector<std::string> files;       //    /www/blog.html ,  /www/here/blabla.php

	// Get all the directories from /www/request.getPath().
	// Remove ?example=hello if found on getPath()
	// Remove the "." / "..", put directories in directories, files in files
	dpdf = opendir(ctx.getSysPath().c_str());

	if (dpdf == NULL)
		return _status_page_response(404, ctx);
    
    while (true)
    {
		epdf = readdir(dpdf);
        if (epdf == NULL)
            break ;
        std::string name = epdf->d_name;
        if (name == "." || name == "..")
            continue ;
        if (epdf->d_type != DT_DIR)	// If not a folder
            files.push_back(name);
        else
            directories.push_back(name + "/");
	}

	closedir(dpdf);

	// Sort alphabetical the files and directories
	std::sort(directories.begin(),directories.end(),compareFunction);
	std::sort(files.begin(),files.end(),compareFunction);

    std::stringstream ss;
    ss << "<html>"
        << "<head><title>Index of " << ctx.endpoint << "</title></head>"
        << "<body><h1>Index of " << ctx.endpoint << "</h1><hr><pre>"
        << "<a href=\"..\">../</a>\n";

    // Print the directories
	for (std::vector<std::string>::iterator it = directories.begin(); it != directories.end(); it++)
    	ss << "<a href=\"http://localhost:" << ctx.server_port << ctx.endpoint << "/" << *it << "\">" << *it << "</a>\n";

    // Print the files
	for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); it++)
    	ss << "<a href=\"http://localhost:" << ctx.server_port << ctx.endpoint << "/" << *it << "\">" << *it << "</a>\n";

    ss << "</pre><hr></body></html>";

    HTTPResponse response;
    response.setHeader("Content-Type", "text/html");
    response.setBody(ss.str());
    return response;
}

HTTPResponse HTTPServer::_redirect_response(const Context& ctx) const
{
    HTTPResponse response;
    response.setHeader("Location", ctx.redirect_path);
    response.setStatus(ctx.redirect_status);
    return response;
}

HTTPResponse HTTPServer::_status_page_response(const HTTPStatus& status, const Context& ctx) const
{
    HTTPResponse response;
    response.setHeader("Content-Type", "text/html");
    response.setStatus(status);

    std::map<int, std::string>::const_iterator it = ctx.error_page.find(status.getCode());
    if (it != ctx.error_page.end())
    {
        std::ifstream ifs(it->second.c_str(), std::ifstream::in);
        if (ifs.good())
        {
            response.setBody(ifs);
            ifs.close();
            return response;
        }
    }

    // Backup
    std::stringstream ss;
    ss << "<h1>" << status << "</h1>";
    response.setBody(ss.str());
    return response;
}

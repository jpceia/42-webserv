/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 17:30:40 by jceia             #+#    #+#             */
/*   Updated: 2022/03/24 02:49:01 by jpceia           ###   ########.fr       */
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

    bool finished = conn->recvChunk();
    if (finished) // Finished receiving the request
    {
        HTTPRequest request = conn->getRequest();

        #ifdef DEBUG
        std::cout << "Request: " << std::endl;
        request.print();
        #endif


        // Get the server block
        configServerBlock server_block = conn->getServerBlock(request.getHeader("Host"));

        // Get correct location config for a given path
        configLocationBlock location_block = server_block.getLocationByPath(request.getPath());

        // Construct the context
        Context ctx;
        ctx.error_page = location_block.getErrorPage();
        ctx.max_body_size = location_block.getClientMaxBodySize();
        ctx.root = location_block.getRoot();
        ctx.rel_path = request.getEndpoint().substr(location_block.getLocationPath().size());
        ctx.base_path = ctx.root + "/" + ctx.rel_path;
        ctx.index = location_block.getIndex();
        ctx.autoindex = location_block.getAutoIndex();
        ctx.allowed_methods = location_block.getMethods();
        ctx.redirect_status = location_block.getRedirectStatus();
        ctx.redirect_path = location_block.getRedirectPath();
        ctx.cgi = location_block.getCgi();
        ctx.upload_path = location_block.getUpload();
        ctx.server_addr = connection->getServerIP();
        ctx.client_addr = connection->getClientIP();
        ctx.server_port = connection->getServerPort();
        ctx.client_port = connection->getClientPort();
        
        if (server_block.getServerName().size() > 0)
        {
            ctx.server_name = server_block.getServerName().front();
            ctx.host_port = ctx.server_name;
        }
        else
        {
            ctx.server_name = "";
            ctx.host_port = ctx.server_addr;
        }

        if (ctx.server_port != 80)
            ctx.host_port += ":" + ft_itos(ctx.server_port);

        // build the response
        HTTPResponse response = _response(conn->getRequest(), ctx);

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
    ctx.path = ctx.base_path;
    if (is_dir(ctx.path))
    {
        // std::cout << "path is a directory" << std::endl;
        bool found = false;
        for (std::vector<std::string>::const_iterator it = ctx.index.begin();
            it != ctx.index.end(); ++it)
        {
            // std::cout << "path: " << ctx.base_path << "\t index:" << *it << std::endl;
            if (is_readable_file(ctx.path + "/" + *it))
            {
                ctx.path = ctx.base_path + "/" +  *it;
                found = true;
                break ;
            }
        }
        if (!found)
        {
            if (ctx.autoindex == "on")
                return _autoindex_response(ctx, request);
            else
                return _status_page_response(404, ctx);
        }
    }
    else if (!is_readable_file(ctx.path))
        return _status_page_response(404, ctx);

    // CGI
    map_str_str::const_iterator it = ctx.cgi.find(_get_file_extension(ctx.path));
    if (it != ctx.cgi.end())
        return _cgi_response(it->second, request, ctx);

    // static file
    return _static_response(ctx);
}

HTTPResponse HTTPServer::_static_response(const Context& ctx) const
{
    // Static page
    std::ifstream ifs(ctx.path.c_str(), std::ifstream::in);
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

HTTPServer::map_str_str HTTPServer::_get_cgi_env(const HTTPRequest& request, const Context& ctx) const
{   
    // http://www.wijata.com/cgi/cgispec.html#4.0
    
    map_str_str env;

    /*
    This variable is specific to requests made via the HTTP scheme.

    If the script-URI required access authentication for external access,
    then the server SHOULD set the value of this variable from the
    'auth-scheme' token in the request's "Authorization" header field.,
    otherwise it is set to NULL.

        AUTH_TYPE   = "" | auth-scheme
        auth-scheme = "Basic" | token

    Servers SHOULD provide this meta-variable to scripts if the request header
    included an "Authorization" field.

    https://developer.mozilla.org/en-US/docs/Web/HTTP/Authentication
    */
    std::string auth = request.getHeader("Authorization");
    if (!auth.empty())
    {
        std::stringstream ss (auth);
        std::string auth_scheme;
        ss >> auth_scheme;
        env["AUTH_TYPE"] = auth_scheme;
    }

    /*
    This meta-variable is set to the size of the content-body entity attached
    to the request, if any, in decimal number of octets. If no data are attached,
    then this meta-variable is either NULL or not defined.
    The syntax is the same as for the HTTP "Content-Length" header field
    (section 14.14, HTTP/1.1 specification [8]).

        CONTENT_LENGTH = "" | 1*digit
  
    Servers MUST provide this meta-variable to scripts if the request was
    accompanied by a content-body entity.
    */
    size_t content_length = request.getBody().size();
    if (content_length > 0)
        env["CONTENT_LENGTH"] = ft_itos(content_length);
    else
        env["CONTENT_LENGTH"] = "";
    
    /*
    If the request includes a content-body, CONTENT_TYPE is set to the Internet
    Media Type [9] of the attached entity if the type was provided via a
    "Content-type" field in the request header
    */
    env["CONTENT_TYPE"] = request.getHeader("Content-Type");
    
    env["DOCUMENT_ROOT"] = ctx.root;
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["PATH_INFO"] = ctx.path;
    env["PATH_TRANSLATED"] = ctx.path;
    env["SCRIPT_NAME"] = ctx.path;
    env["SCRIPT_FILENAME"] = ctx.path;
    env["REDIRECT_STATUS"] = ctx.redirect_path;
    env["REQUEST_URI"] = ctx.path;
    env["HTTP_X_SECRET_HEADER_FOR_TEST"] = "1";
    env["QUERY_STRING"] = request.getQueryString();

    // HTTP info
    env["HTTP_ACCEPT"] = request.getHeader("Accept");
    env["HTTP_ACCEPT_CHARSET"] = request.getHeader("Accept-Charset");
    env["HTTP_ACCEPT_ENCODING"] = request.getHeader("Accept-Encoding");
    env["HTTP_ACCEPT_LANGUAGE"] = request.getHeader("Accept-Language");
    env["HTTP_FORWARDED"] = request.getHeader("Forwarded");
    env["HTTP_PROXY_AUTHORIZATION"] = request.getHeader("Proxy-Authorization");
    env["HTTP_USER_AGENT"] = request.getHeader("User-Agent");
    env["HTTP_CONNECTION"] = request.getHeader("Connection");
    env["HTTP_COOKIE"] = request.getHeader("Cookie");

    std::string host = request.getHeader("Hostname");
    if (host.empty())
        host = ctx.server_addr;
    env["HTTP_HOST"] = host;

    {
        std::stringstream ss;
        ss << request.getMethod();
        env["REQUEST_METHOD"] = ss.str();
    }

    /*
    The SERVER_PORT meta-variable is set to the port on which thise request was
    received, as used in the <port> part of the script-URI.
        SERVER_PORT = 1*digit
    
    If the <port> portion of the script-URI is blank, the actual port number upon
    which the request was received MUST be supplied.

    Servers MUST provide this meta-variable to scripts.
    */
    env["SERVER_ADDR"] = ctx.server_addr +  ft_itos(ctx.server_port);
    env["SERVER_PORT"] = ft_itos(ctx.server_port);
    env["SERVER_PROTOCOL"] = request.getVersion();
    env["SERVER_SOFTWARE"] = "webserv/1.0";
    if (ctx.server_name.empty())
        env["SERVER_NAME"] = env["SERVER_ADDR"] + ":" + env["SERVER_PORT"];
    else
        env["SERVER_NAME"] = ctx.server_name;
    
    /*
    The IP address of the agent sending the request to the server.
    This is not necessarily that of the client (such as if the request came
    through a proxy).
    */
    env["REMOTE_ADDR"] = ctx.client_addr;
    env["REMOTE_PORT"] = ft_itos(ctx.client_port);

    return env;
}

// Client_port or Server_port is wrong
// Need to change it and then change it here:
// ss << "\t\t<p><a href=\"http://" + ctx.client_addr + ":" <<\
// "8080" << dirName + "/" + dirEntry + "\">" + dirEntry + "</a></p>\n";
HTTPResponse HTTPServer::_autoindex_response(const Context& ctx, const HTTPRequest& request) const
{
	DIR *dpdf;
	struct dirent *epdf = NULL;

	std::vector<std::string> directories; //    /www/blog ,       /www/here/blabla
	std::vector<std::string> files;       //    /www/blog.html ,  /www/here/blabla.php

	// Get all the directories from /www/request.getPath().
	// Remove ?example=hello if found on getPath()
	// Remove the "." / "..", put directories in directories, files in files
	std::string path_treated = request.getPath().substr(0, request.getPath().find("?"));
	std::string full_path = ctx.root + path_treated;
	dpdf = opendir(full_path.c_str());

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
    ss << "<html><head><title>Index of " << path_treated << "</title></head><body><h1>Index of " << path_treated << "</h1><hr><pre>";
    ss << "<a href=\"..\">../</a>\n";

    // Print the directories
	for (std::vector<std::string>::iterator it = directories.begin(); it != directories.end(); it++)
    	ss << "<a href=\"http://" << ctx.host_port << path_treated <<  *it << "\">" << *it << "</a>\n";

    // Print the files
	for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); it++)
    	ss << "<a href=\"http://" << ctx.host_port << path_treated << *it << "\">" << *it << "</a>\n";

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

HTTPResponse HTTPServer::_upload_response(const HTTPRequest& request, const Context& ctx) const
{
    HTTPMethod method = request.getMethod();
    if (method == DELETE)
        return _delete_response(ctx);
    if (method == GET)
        return _static_response(ctx);
    if (method != POST && method != PUT)
        return _status_page_response(405, ctx);

    HTTPResponse response;
    response.setHeader("Content-Type", "text/html");
    std::string path = ctx.upload_path + ctx.rel_path;
    std::cout << "Upload path: " << path << std::endl;
    // write to file
    std::ofstream ofs(path.c_str(), std::ios::binary | std::ios::trunc);
    if (!ofs.good())
        return _status_page_response(500, ctx);
    std::string body = request.getBody();
    ofs.write(body.c_str(), body.size());
    ofs.close();

    response.setBody("Upload Successfull");
    return response;
}

HTTPResponse HTTPServer::_delete_response(const Context& ctx) const
{
    std::string path = ctx.upload_path + ctx.rel_path;
    std::cout << "Delete path: " << path << std::endl;

    if (std::remove(path.c_str()) != 0)
        return _status_page_response(500, ctx);

    HTTPResponse response = _status_page_response(202, ctx);
    response.setBody("Delete Successfull");
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

std::string HTTPServer::_get_file_extension(const std::string& path) const
{
    std::string ext;
    size_t pos = path.find_last_of('.');
    if (pos != std::string::npos)
        ext = path.substr(pos + 1);
    return ext;
}

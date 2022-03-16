/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 17:30:40 by jceia             #+#    #+#             */
/*   Updated: 2022/03/16 13:00:56 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSERVER_CPP
# define HTTPSERVER_CPP

# include "TCPServer.hpp"
# include "HTTPRequest.hpp"
# include "HTTPResponse.hpp"
# include "HTTPConnection.hpp"
# include "configFile.hpp"
# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>

struct Context
{
    std::string path;
    std::map<int, std::string> error_page;
    unsigned long int max_body_size;
    std::string root;
    std::string autoindex;
    std::vector<std::string> index;
    std::vector<HTTPMethod> allowed_methods;
    int redirect_status;
    std::string redirect_path;
    std::map<std::string, std::string> cgi;
    std::string upload_path;
    std::string server_name;
    std::string server_addr;
    std::string client_addr;
    int server_port;
    int client_port;
};

class HTTPServer : public TCPServer
{
public:
    HTTPServer(configFile config, int timeout = -1);
    virtual ~HTTPServer();

    void init();

protected:
    int _handle_client_recv(TCPConnection* connection);
    int _handle_client_send(TCPConnection* connection);

private:
    HTTPResponse _response(const HTTPRequest& request, Context& ctx);
    HTTPResponse _cgi_response(const std::string& cmd, const HTTPRequest& request, const Context& ctx);
    //HTTPResponse _static_response(const HTTPRequest& request, const Context& ctx);
    //HTTPResponse _redirect_response(const HTTPRequest& request, const Context& ctx) const;
    //HTTPResponse _upload_response(const HTTPRequest& request, const Context& ctx);
    HTTPResponse _autoindex_response(const Context& ctx) const;
    HTTPResponse _redirect_response(const Context& ctx) const;
    HTTPResponse _error_page_response(int code, const std::string& msg, const Context& ctx) const;

    configFile _config;
};

#endif

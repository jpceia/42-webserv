/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 17:30:40 by jceia             #+#    #+#             */
/*   Updated: 2022/03/24 02:36:52 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSERVER_HPP
# define HTTPSERVER_HPP

# include "TCPServer.hpp"
# include "HTTPRequest.hpp"
# include "HTTPResponse.hpp"
# include "HTTPConnection.hpp"
# include "configFile.hpp"
# include "debug.hpp"
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
    std::string rel_path;
    std::string base_path;
    std::string autoindex;
    std::vector<std::string> index;
    std::set<HTTPMethod> allowed_methods;
    int redirect_status;
    std::string redirect_path;
    std::map<std::string, std::string> cgi;
    std::string host_port;
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
    typedef std::map<std::string, std::string> map_str_str;

    HTTPServer(configFile config, int timeout = -1);
    virtual ~HTTPServer();

    void init();

protected:
    void _handle_client_recv(TCPConnection* connection, short& event);
    void _handle_client_send(TCPConnection* connection, short& event);

private:
    // Non-copyable
    HTTPServer(const HTTPServer& rhs);
    HTTPServer &operator=(const HTTPServer& rhs);

    HTTPResponse _response(const HTTPRequest& request, Context& ctx);
    HTTPResponse _cgi_response(const std::string& cmd, const HTTPRequest& request, const Context& ctx);
    HTTPResponse _static_response(const Context& ctx) const;
    HTTPResponse _autoindex_response(const Context& ctx, const HTTPRequest& request) const;
    HTTPResponse _upload_response(const HTTPRequest& request, const Context& ctx) const;
    HTTPResponse _delete_response(const Context& ctx) const;
    HTTPResponse _redirect_response(const Context& ctx) const;
    HTTPResponse _error_page_response(const HTTPStatus& status, const Context& ctx) const;

    // Helpers
    std::string _get_file_extension(const std::string& path) const;
    map_str_str _get_cgi_env(const HTTPRequest& request, const Context& ctx) const;

    configFile _config;
};

#endif

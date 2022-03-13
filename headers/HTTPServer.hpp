/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 17:30:40 by jceia             #+#    #+#             */
/*   Updated: 2022/03/09 18:53:37 by jpceia           ###   ########.fr       */
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
    HTTPResponse _not_found_response();
    HTTPResponse _method_not_allowed_response();
    HTTPResponse _body_too_large_response();

    HTTPResponse _build_response(const HTTPRequest& request, Context& ctx);
    HTTPResponse _build_cgi_response(const HTTPRequest& request, const Context& ctx);

    std::string _root; // TODO: remove
    std::string _name; // TODO: remove
    size_t _max_body_size; // TODO: remove
    std::vector<std::string> _index; // TODO: remove
    std::vector<HTTPMethod> _allowed_methods; // TODO: remove
    configFile _config;
};

#endif

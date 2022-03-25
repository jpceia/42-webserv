/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServerHelpers.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 04:13:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/25 15:27:38 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPServer.hpp"
#include "utils.hpp"


std::string HTTPServer::_get_file_extension(const std::string& path) const
{
    std::string ext;
    size_t pos = path.find_last_of('.');
    if (pos != std::string::npos)
        ext = path.substr(pos + 1);
    return ext;
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

    std::string path = ctx.getSysPath();
    env["PATH_INFO"] = path;
    env["PATH_TRANSLATED"] = path;
    env["SCRIPT_NAME"] = path;
    env["SCRIPT_FILENAME"] = path;
    env["REDIRECT_STATUS"] = ctx.redirect_path;
    env["REQUEST_URI"] = path;
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

Context HTTPServer::_get_context(const HTTPStatefulConnection *conn) const
{
    Context ctx;

    HTTPRequest request = conn->getRequest();
    // Get the server block
    configServerBlock server_block = conn->getServerBlock(request.getHeader("Host"));

    // Get correct location config for a given path
    configLocationBlock location_block = server_block.getLocationByPath(request.getPath());

    ctx.error_page = location_block.getErrorPage();
    ctx.max_body_size = location_block.getClientMaxBodySize();
    ctx.root = location_block.getRoot();
    ctx.endpoint = request.getEndpoint();
    ctx.location_path = location_block.getLocationPath();
    ctx.upload = location_block.getUpload() == "on";
    ctx.autoindex = location_block.getAutoIndex() == "on";
    if (location_block.getIsRootFromLocationBlock())
    {
        ctx.sys_rel_path = ctx.endpoint.substr(ctx.location_path.size());
        if (ctx.sys_rel_path.empty())
            ctx.sys_rel_path = "/";
        else if (ctx.sys_rel_path[0] != '/')
            ctx.sys_rel_path = "/" + ctx.sys_rel_path;
    }
    else
    {
        ctx.sys_rel_path = ctx.endpoint;
    }
    ctx.index = location_block.getIndex();
    ctx.allowed_methods = location_block.getMethods();
    ctx.redirect_status = location_block.getRedirectStatus();
    ctx.redirect_path = location_block.getRedirectPath();
    ctx.cgi_bin = location_block.getCgi();
    ctx.server_addr = conn->getServerIP();
    ctx.client_addr = conn->getClientIP();
    ctx.server_port = conn->getServerPort();
    ctx.client_port = conn->getClientPort();
    
    std::string server_name = "";
    if (server_block.getServerName().size() > 0)
        server_name = server_block.getServerName().front();
    if (server_name.empty())
        ctx.host_port = ctx.server_addr;
    else
        ctx.server_name = "";
    if (ctx.server_port != 80)
        ctx.host_port += ":" + ft_itos(ctx.server_port);

    return ctx;
}

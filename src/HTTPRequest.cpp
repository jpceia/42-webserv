/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 15:33:45 by jceia             #+#    #+#             */
/*   Updated: 2022/02/23 05:45:56 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "HTTPRequest.hpp"
# include <sstream>

HTTPMethod parseHTTPMethod(std::string const &method)
{
    if (method == "GET")
        return GET;
    else if (method == "POST")
        return POST;
    else if (method == "DELETE")
        return DELETE;
    return UNKNOWN;
}

std::string strHTTPMethod(HTTPMethod method)
{
    if (method == GET)
        return "GET";
    else if (method == POST)
        return "POST";
    else if (method == DELETE)
        return "DELETE";
    return "UNKNOWN";
}

HTTPRequest::HTTPRequest() :
    _path(""),
    _body(""),
    _version("")
{
}

HTTPRequest::~HTTPRequest()
{
}

HTTPRequest& HTTPRequest::operator=(const HTTPRequest &rhs)
{
    if (this != &rhs)
    {
        this->_method = rhs._method;
        this->_path = rhs._path;
        this->_body = rhs._body;
    }
    return *this;
}

const char* HTTPRequest::ParseException::what(void) const throw()
{
    return "Error parsing HTTP Request";
}

std::istream &operator>>(std::istream &is, HTTPRequest &request)
{
    std::string line;
    std::getline(is, line);
    std::stringstream ss(line.substr(0, line.length() - 1));
    std::string method;
    ss >> method >> request._path >> request._version; // parse start line
    if (!ss.eof())
        throw HTTPRequest::ParseException(); // invalid start line
    request._method = parseHTTPMethod(method);
    if (request._method == UNKNOWN)
        throw HTTPRequest::ParseException(); // invalid method
    // Parse requests
    while (std::getline(is, line))
    {
        line = line.substr(0, line.length() - 1);
        if (line.empty())
            break;
        size_t pos = line.find(':');
        if (pos == std::string::npos)
            throw HTTPRequest::ParseException(); // invalid header
        request._headers[line.substr(0, pos)] = line.substr(pos + 2);      
    }
    // Parse body
    while (std::getline(is, line))
    {
        request._body += line.substr(0, line.length() - 1);
        request._body += "\n";
    }
    return is;
}

std::ostream &operator<<(std::ostream &out, const HTTPRequest &request)
{
    out << strHTTPMethod(request._method) << " " << request._path << " " << request._version << "\r\n";
    for (std::map<std::string, std::string>::const_iterator it = request._headers.begin();
        it != request._headers.end(); ++it)
        out << it->first << ": " << it->second << "\r\n";
    out << "\r\n" << request._body;
    return out;
}

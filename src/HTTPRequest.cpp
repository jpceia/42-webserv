/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 15:33:45 by jceia             #+#    #+#             */
/*   Updated: 2022/03/19 15:55:47 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "HTTPRequest.hpp"
# include "utils.hpp"
# include <sstream>



HTTPRequest::HTTPRequest() :
    _endpoint(""),
    _query(""),
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
        _method = rhs._method;
        _endpoint = rhs._endpoint;
        _query = rhs._query;
        _body = rhs._body;
    }
    return *this;
}

const char* HTTPRequest::ParseException::what(void) const throw()
{
    return "Error parsing HTTP Request";
}

static std::string& drop_carriage_return(std::string& s)
{
    size_t pos = s.length() - 1;
    if (s[pos] != '\r')
        throw HTTPRequest::ParseException();
    s = s.substr(0, pos);
    return s;
}

std::istream &operator>>(std::istream &is, HTTPRequest &request)
{
    std::string line;

    // Start line
    {
        std::getline(is, line);
        std::stringstream ss(drop_carriage_return(line));
        std::string method;
        std::string path;
        ss >> method >> path >> request._version;
        request.setPath(path);
        request.setMethod(method);
        if (!ss.eof())
            throw HTTPRequest::ParseException();
    }
    
    // Headers
    while (std::getline(is, line))
    {
        drop_carriage_return(line);
        if (line.empty())
            break;
        request.addHeader(line);
    }
    
    // Body
    while (std::getline(is, line))
    {
        request._body += drop_carriage_return(line);
        request._body += "\n";
    }
    return is;
}

std::ostream &operator<<(std::ostream &out, const HTTPRequest &request)
{
    out << request._method << " "
        << request.getPath() << " "
        << request._version << "\r\n";
    for (std::map<std::string, std::string>::const_iterator it = request._headers.begin();
        it != request._headers.end(); ++it)
        out << it->first << ": " << it->second << "\r\n";
    out << "\r\n" << request._body;
    return out;
}

std::istream &operator>>(std::istream &is, HTTPMethod &method)
{
    std::string s;
    is >> s;
    if (s == "GET")
        method = GET;
    else if (s == "POST")
        method = POST;
    else if (s == "PUT")
        method = PUT;
    else if (s == "DELETE")
        method = DELETE;
    else if (s == "HEAD")
        method = HEAD;
    else
        throw HTTPMessage::ParseException();
    return is;
}

std::ostream &operator<<(std::ostream &out, const HTTPMethod &method)
{
    switch (method)
    {
    case GET:
        out << "GET";
        break ;
    case POST:
        out << "POST";
        break ;
    case PUT:
        out << "PUT";
        break ;
    case DELETE:
        out << "DELETE";
        break ;
    case HEAD:
        out << "HEAD";
        break ;
    default:
        throw std::runtime_error("Unkown HTTP method");
    }
    return out;
}

std::string HTTPRequest::getPath() const
{
    if (_query.empty())
        return _endpoint;
    return _endpoint + "?" + _query;
}

std::string HTTPRequest::getBody() const
{
    return _body;
}

std::string HTTPRequest::getQueryString() const
{
    return _query;
}

std::string HTTPRequest::getEndpoint() const
{
    return _endpoint;
}

HTTPMethod HTTPRequest::getMethod() const
{
    return _method;
}

std::string HTTPRequest::getHeader(const std::string& key) const
{
    std::map<std::string, std::string>::const_iterator it = _headers.find(key);
    if (it == _headers.end())
        return "";
    return it->second;
}

std::map<std::string, std::string> HTTPRequest::getHeaders() const
{
	return _headers;
}

void HTTPRequest::setPath(const std::string& path)
{
    size_t pos = path.find('?');
    if (pos != std::string::npos)
    {
        _endpoint = path.substr(0, pos);
        _query = path.substr(pos + 1);
    }
    else
    {
        _endpoint = path;
        _query = "";
    }
}

void HTTPRequest::setMethod(const std::string& method)
{
    std::stringstream ss(method);
    ss >> _method;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jceia <jceia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 15:33:45 by jceia             #+#    #+#             */
/*   Updated: 2022/03/07 18:03:30 by jceia            ###   ########.fr       */
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
        this->_method = rhs._method;
        this->_endpoint = rhs._endpoint;
        this->_query = rhs._query;
        this->_body = rhs._body;
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
        std::string url;
        ss >> method >> url >> request._version;
        request.setUrl(url);
        if (!ss.eof())
            throw HTTPRequest::ParseException();
        request._method = HTTPRequest::parseMethod(method);
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
    out << HTTPRequest::strMethod(request._method) << " ";
    out << request.getUrl() << " " << request._version << "\r\n";
    for (std::map<std::string, std::string>::const_iterator it = request._headers.begin();
        it != request._headers.end(); ++it)
        out << it->first << ": " << it->second << "\r\n";
    out << "\r\n" << request._body;
    return out;
}

HTTPMethod HTTPRequest::parseMethod(std::string const &s)
{
    if (s == "GET")
        return GET;
    if (s == "POST")
        return POST;
    if (s == "DELETE")
        return DELETE;
    throw HTTPRequest::ParseException();
}

std::string HTTPRequest::strMethod(HTTPMethod method)
{
    switch (method)
    {
    case GET:
        return "GET";
    case POST:
        return "POST";
    case DELETE:
        return "DELETE";
    default:
        throw std::runtime_error("Unkown HTTP method");
    }
}

void HTTPRequest::addHeader(const std::string& s)
{
    size_t pos = s.find(':');
    if (pos == std::string::npos)
        throw HTTPRequest::ParseException();
    std::string key = s.substr(0, pos);
    // find first non-whitespace character after ':'
    pos = s.find_first_not_of(" \t", pos + 1);
    if (pos == std::string::npos)
        throw HTTPRequest::ParseException();
    std::string value = s.substr(pos);
    this->addHeader(key, value);
}

void HTTPRequest::addHeader(const std::string& key, const std::string& value)
{
    _headers[key] = value;
}

std::string HTTPRequest::getVersion() const
{
    return _version;
}

std::string HTTPRequest::getUrl() const
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

void HTTPRequest::setUrl(const std::string& url)
{
    size_t pos = url.find('?');
    if (pos != std::string::npos)
    {
        _endpoint = url.substr(0, pos);
        _query = url.substr(pos + 1);
    }
    else
    {
        _endpoint = url;
        _query = "";
    }
}

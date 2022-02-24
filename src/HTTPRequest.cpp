/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 15:33:45 by jceia             #+#    #+#             */
/*   Updated: 2022/02/24 06:53:37 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "HTTPRequest.hpp"
# include "utils.hpp"
# include <sstream>



HTTPRequest::HTTPRequest() :
    _path(""),
    _body(""),
    _version(""),
    _parse_status(PARSE_START),
    _buf("")
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

static void drop_carriage_return(std::string& s)
{
    size_t pos = s.length() - 1;
    if (s[pos] != '\r')
        throw HTTPRequest::ParseException();
    s = s.substr(0, pos);
}

std::istream &operator>>(std::istream &is, HTTPRequest &request)
{
    std::string line;
    std::getline(is, line);
    drop_carriage_return(line);
    std::stringstream ss(line);
    std::string method;
    ss >> method >> request._path >> request._version;
    if (!ss.eof())
        throw HTTPRequest::ParseException(); // invalid start line
    request._method = HTTPRequest::parseMethod(method);
    // Parse headers
    while (std::getline(is, line))
    {
        drop_carriage_return(line);
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
        drop_carriage_return(line);
        request._body += line;
        request._body += "\n";
    }
    request._parse_status = PARSE_COMPLETE;
    return is;
}

ParseStatus HTTPRequest::parse(const std::string& s)
{
    size_t pos;
    
    if (_parse_status == PARSE_START)
    {
        pos = s.find("\r\n");
        if (pos != std::string::npos)
        {
            _buf += s.substr(0, pos);
            _parse_status = PARSE_HEADER;
            std::stringstream ss(_buf);
            std::string method;
            ss >> method >> _path >> _version; // parse start line
            if (!ss.eof())
                throw HTTPRequest::ParseException();
            _method = HTTPRequest::parseMethod(method);
            _buf = "";
            return this->parse(s.substr(pos + 2));
        }
        _buf += s;
    }
    else if (_parse_status == PARSE_HEADER)
    {
        pos = s.find("\r\n");
        if (pos != std::string::npos)
        {
            _buf += s.substr(0, pos);
            if (_buf.empty())
            {
                _parse_status = PARSE_BODY;
                return this->parse(s.substr(pos + 2));
            }
            size_t i = _buf.find(':');
            if (i == std::string::npos)
                throw HTTPRequest::ParseException();
            _headers[_buf.substr(0, i)] = _buf.substr(i + 2);
            _buf = "";
            return this->parse(s.substr(pos + 2));
        }
        _buf += s;
    }
    else if (_parse_status == PARSE_BODY)
    {
        if (s.empty())
            return PARSE_COMPLETE;
        _body += s;
        std::map<std::string, std::string>::const_iterator it = _headers.find("Content-Length");
        if (it == _headers.end())
            throw HTTPRequest::ParseException();
        size_t len = stoi(it->second);
        if (_body.length() > len)
            throw HTTPRequest::ParseException();
        if (_body.length() == len)
            return PARSE_COMPLETE;
    }
    return _parse_status;
}

std::ostream &operator<<(std::ostream &out, const HTTPRequest &request)
{
    out << HTTPRequest::strMethod(request._method) << " ";
    out << request._path << " " << request._version << "\r\n";
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

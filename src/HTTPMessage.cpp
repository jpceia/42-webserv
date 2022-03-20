/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPMessage.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 00:50:16 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/20 01:29:42 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPMessage.hpp"
#include <sstream>
#include <fstream>

HTTPMessage::HTTPMessage() :
    _version("HTTP/1.1"),
    _body("")
{
}

HTTPMessage::HTTPMessage(const HTTPMessage &rhs)
{
    if (this != &rhs)
    {
        _version = rhs._version;
        _body = rhs._body;
        _headers = rhs._headers;
    }
}

HTTPMessage::~HTTPMessage()
{
}

HTTPMessage& HTTPMessage::operator=(const HTTPMessage &rhs)
{
    if (this != &rhs)
    {
        _version = rhs._version;
        _body = rhs._body;
        _headers = rhs._headers;
    }
    return *this;
}

// Getters
std::string HTTPMessage::getVersion() const
{
    return _version;
}

std::map<std::string, std::string> HTTPMessage::getHeaders() const
{
    return _headers;
}

std::string HTTPMessage::getHeader(const std::string& key) const
{
    std::map<std::string, std::string>::const_iterator it = _headers.find(key);
    if (it == _headers.end())
        return "";
    return it->second;
}

std::string HTTPMessage::getBody() const
{
    return _body;
}

// Setters
void HTTPMessage::setVersion(const std::string& version)
{
    _version = version;
}

void HTTPMessage::setHeader(const std::string& key, const std::string& value)
{
    _headers[key] = value;
}

// Parses a line with format 'key: value' and adds it to the headers map
void HTTPMessage::setHeader(const std::string& line)
{
    size_t pos = line.find(':');
    if (pos == std::string::npos)
        throw HTTPMessage::ParseException();
    std::string key = line.substr(0, pos);
    // find first non-whitespace character after ':'
    pos = line.find_first_not_of(" \t", pos + 1);
    if (pos == std::string::npos)
        throw HTTPMessage::ParseException();
    std::string value = line.substr(pos);
    this->setHeader(key, value);
}

void HTTPMessage::setBody(const std::string& body)
{
    _body = body;
}

void HTTPMessage::setBody(const std::ifstream& f)
{
    std::stringstream ss;
    ss << f.rdbuf();
    this->setBody(ss.str());
}

const char* HTTPMessage::ParseException::what(void) const throw()
{
    return "Error parsing HTTP Message";
}

std::string& _drop_carriage_return(std::string& s)
{
    size_t pos = s.length() - 1;
    if (s[pos] != '\r')
        throw HTTPMessage::ParseException();
    s = s.substr(0, pos);
    return s;
}

// Parses headers and body
std::istream &operator>>(std::istream &is, HTTPMessage &msg)
{
    std::string line;

    // Headers
    while (std::getline(is, line))
    {
        _drop_carriage_return(line);
        if (line.empty())
            break;
        msg.setHeader(line);
    }
    
    // Body
    while (std::getline(is, line))
    {
        msg._body += _drop_carriage_return(line);
        msg._body += "\n";
    }
    return is;
}

// Write headers and body to an output stream
std::ostream &operator<<(std::ostream &out, const HTTPMessage &msg)
{
    for (std::map<std::string, std::string>::const_iterator it = msg._headers.begin();
        it != msg._headers.end(); ++it)
        out << it->first << ": " << it->second << "\r\n";
    out << "\r\n" << msg._body;
    return out;
}

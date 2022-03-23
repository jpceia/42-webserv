/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPMessage.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 00:50:16 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/23 01:20:43 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPMessage.hpp"
#include "utils.hpp"
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

// -----------------------------------------------------------------------------
//                                   Getters
// -----------------------------------------------------------------------------

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

// -----------------------------------------------------------------------------
//                                   Setters
// -----------------------------------------------------------------------------

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

// -----------------------------------------------------------------------------
//                                  Helpers
// -----------------------------------------------------------------------------

void HTTPMessage::printStart() const
{
    std::cout << _version << std::endl;
}

void HTTPMessage::printHeaders() const
{
    for (std::map<std::string, std::string>::const_iterator it = _headers.begin();
        it != _headers.end(); it++)
        std::cout << it->first << ": " << it->second << std::endl;
}

void HTTPMessage::printBody() const
{
    size_t max_size = 250;
    size_t size = _body.size();
    if (size > max_size)
        std::cout << _body.substr(0, max_size) << "..." << std::endl;
    else
        std::cout << _body << std::endl;
    if (size > 0)
        std::cout << "(size: " << _body.size() << ")" << std::endl;
}

void HTTPMessage::print() const
{
    std::cout << "----------------------------------------" << std::endl;
    this->printStart();
    this->printHeaders();
    std::cout << std::endl;
    this->printBody();
    std::cout << "----------------------------------------" << std::endl;
    std::cout << std::endl;
}

const char* HTTPMessage::ParseException::what(void) const throw()
{
    return "Error parsing HTTP Message";
}

std::string& _drop_carriage_return(std::string& s, bool raise)
{
    if (!s.empty())
    {
        size_t pos = s.length() - 1;
        if (s[pos] != '\r')
        {
            if (raise)
                throw HTTPMessage::ParseException();
            else
                return s;
        }
        s = s.substr(0, pos);
    }
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
        msg._body += _drop_carriage_return(line, false);
        msg._body += "\n";
    }
    // Add Content-Length header if not present
    msg.setHeader("Content-Length", ft_itos(msg._body.length()));
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

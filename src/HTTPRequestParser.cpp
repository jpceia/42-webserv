/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 05:15:06 by jpceia            #+#    #+#             */
/*   Updated: 2022/02/24 05:21:27 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequestParser.hpp"
#include "utils.hpp"
#include <sstream>

HTTPRequestParser::HTTPRequestParser() :
    HTTPRequest(),
    _state(PARSE_START),
    _buf(""),
    _content_length(0)
{
}

HTTPRequestParser::HTTPRequestParser(const HTTPRequestParser& rhs) :
    HTTPRequest(rhs),
    _state(rhs._state),
    _buf(rhs._buf),
    _content_length(rhs._content_length)
{
}

HTTPRequestParser::~HTTPRequestParser()
{
}

HTTPRequestParser& HTTPRequestParser::operator=(const HTTPRequestParser& rhs)
{
    HTTPRequest::operator=(rhs);
    if (this != &rhs)
    {
        this->_state = rhs._state;
        this->_buf = rhs._buf;
        this->_content_length = rhs._content_length;
    }
    return *this;
}

ParseState HTTPRequestParser::parse(const std::string& s = "")
{
    _buf += s;
    if (_state == PARSE_START)
    {
        size_t pos = _buf.find("\r\n");
        if (pos != std::string::npos)
        {
            std::stringstream ss(_buf.substr(0, pos));
            _buf = _buf.substr(pos + 2);
            std::string method;
            ss >> method >> _path >> _version; // parse start line
            if (!ss.eof())
                throw HTTPRequest::ParseException();
            _method = HTTPRequestParser::parseMethod(method);
            _state = PARSE_HEADER;
            return this->parse();
        }
    }
    else if (_state == PARSE_HEADER)
    {
        size_t pos = _buf.find("\r\n");
        if (pos != std::string::npos)
        {
            std::string line = _buf.substr(0, pos);
            _buf = _buf.substr(pos + 2);
            if (line.empty())
            {
                if (_method == POST)
                {
                    _state = PARSE_BODY;
                    return this->parse();
                }
                else
                    return PARSE_COMPLETE;
            }
            HTTPRequest::addHeader(line);
            return this->parse();
        }
    }
    else if (_state == PARSE_BODY)
    {
        if (_buf.empty())
            return PARSE_COMPLETE;
        _body += _buf;
        _buf = "";
        std::stringstream ss;
        if (_body.length() > _content_length)
            throw HTTPRequest::ParseException();
        if (_body.length() == _content_length)
            return PARSE_COMPLETE;
    }
    return _state;
}

void HTTPRequestParser::addHeader(const std::string& key, const std::string& value)
{
    _headers[key] = value;
    if (key == "Content-Length")
        _content_length = stoi(value);
}

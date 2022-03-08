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
    _buf += s;  // append last received chunk to buffer
    if (_state == PARSE_START)
    {
        size_t pos = _buf.find("\r\n");

        // only parsing if we have the first line complete
        if (pos != std::string::npos)
        {
            std::stringstream ss(_buf.substr(0, pos));  // pass first line to stream
            _buf = _buf.substr(pos + 2);                // remove first line from buffer
            std::string method;
            std::string path;
            ss >> method >> path >> _version;           // parse start line
            if (!ss.eof())
                throw HTTPRequest::ParseException();
            this->setPath(path);
            this->setMethod(method);
            _state = PARSE_HEADER;
            this->parse();                              // Consume buffer
        }
    }
    else if (_state == PARSE_HEADER)
    {
        size_t pos = _buf.find("\r\n");

        // only parsing if we have at least one header line complete
        if (pos != std::string::npos)
        {
            std::string line = _buf.substr(0, pos);     // get first available line
            _buf = _buf.substr(pos + 2);                // remove first line from buffer
            
            // if the line is empty the header section is over
            if (line.empty())                           
            {
                if (_content_length > 0)                // if we have a content length
                    _state = PARSE_BODY;                // we are now parsing the body
                else
                {
                    _state = PARSE_COMPLETE;            // otherwise we are done
                    return _state;
                }
            }
            else
                HTTPRequest::addHeader(line);
            return this->parse();                       // Consume buffer
        }
    }
    else if (_state == PARSE_BODY)
    {
        _body += _buf;
        _buf = "";                                      // clear buffer
        if (_body.length() > _content_length)
            throw HTTPRequest::ParseException();
        if (_body.length() == _content_length)
            _state = PARSE_COMPLETE;
    }
    return _state;
}

void HTTPRequestParser::addHeader(const std::string& key, const std::string& value)
{
    _headers[key] = value;
    if (key == "Content-Length")
        _content_length = ft_stoi(value);
}

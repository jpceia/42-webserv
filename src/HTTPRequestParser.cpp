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
    _content_length(0),
    _chunked(false)
{
}

HTTPRequestParser::HTTPRequestParser(const HTTPRequestParser& rhs) :
    HTTPRequest(rhs),
    _state(rhs._state),
    _buf(rhs._buf),
    _content_length(rhs._content_length),
    _chunked(rhs._chunked)
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
        _state = rhs._state;
        _buf = rhs._buf;
        _content_length = rhs._content_length;
        _chunked = rhs._chunked;
    }
    return *this;
}

ParseState HTTPRequestParser::getState() const
{
    return _state;
}

ParseState HTTPRequestParser::parse(const std::string& s)
{
    _buf += s;  // append last received chunk to buffer
    if (_state == PARSE_START)
        return _parse_start();
    if (_state == PARSE_HEADER)
        return _parse_headers();
    if (_state == PARSE_BODY)
        return _parse_body();
    return PARSE_COMPLETE;
}

ParseState HTTPRequestParser::_parse_start()
{
    size_t pos = _buf.find("\r\n");

    // only parsing if we have the first line complete
    if (pos == std::string::npos)
        return _state;
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
    return this->parse();                       // Consume buffer
}

ParseState HTTPRequestParser::_parse_headers()
{
    size_t pos = _buf.find("\r\n");

    // only parsing if we have at least one header line complete
    if (pos == std::string::npos)
        return _state;
        
    std::string line = _buf.substr(0, pos);     // get first available line
    _buf = _buf.substr(pos + 2);                // remove first line from buffer
    
    // if the line is empty the header section is over
    if (line.empty())                           
    {
        if (_method == POST || _method == PUT)  // if the methods is POST or PUT
            _state = PARSE_BODY;                // we are now parsing the body
        else
        {
            _state = PARSE_COMPLETE;            // otherwise we are done
            return _state;
        }
    }
    else
        HTTPMessage::setHeader(line);
    return this->parse();                       // Consume buffer
}

ParseState HTTPRequestParser::_parse_body()
{
    if (_chunked)
        return _parse_chunked_body();
    // else
    _body += _buf;
    _buf = "";  // clear buffer
    size_t current_body_len = _body.size();
    if (current_body_len > _content_length)
        throw HTTPRequest::ParseException();
    if (current_body_len == _content_length)
        _state = PARSE_COMPLETE;
    return _state;
}

ParseState HTTPRequestParser::_parse_chunked_body()
{
    size_t current_body_len = _body.size();
    if (_content_length < current_body_len)
        throw HTTPRequest::ParseException();
    // else
    if (_content_length > current_body_len)
    {
        // consume buffer up to _content_length
        size_t n = _content_length - current_body_len;
        if (_buf.size() < n)
        {
            _body += _buf;
            _buf = "";
            return _state;
        }
        // else
        _body += _buf.substr(0, n);
        _buf = _buf.substr(n);
        return this->parse(); // consume buffer
    }
    // else
    return _parse_next_chunk();
}

ParseState HTTPRequestParser::_parse_next_chunk()
{
    // check if the body is complete or if we need to consume a new chunk
    size_t pos = _buf.find("\r\n");
    if (pos == 0)
    {
        _buf = _buf.substr(2);
        pos = _buf.find("\r\n");
    }
    if (pos == std::string::npos)
        return _state;  // fetch more data
    std::string line = _buf.substr(0, pos);
    _buf = _buf.substr(pos + 2);
    if (line.empty())
    {
        _state = PARSE_COMPLETE;
        return _state;
    }
    // else
    // parse number of bytes in chunk
    std::stringstream ss(line);
    int chunk_size;
    // parse chunk size in hexadecimal format
    ss >> std::hex >> chunk_size;
    if (!ss.eof())
        throw HTTPRequest::ParseException();
    if (chunk_size == 0)
    {
        _state = PARSE_COMPLETE;
        return _state;
    }
    _content_length += chunk_size;
    return this->parse(); // consume buffer
}

void HTTPRequestParser::setHeader(const std::string& key, const std::string& value)
{
    HTTPMessage::setHeader(key, value);
    if (key == "Content-Length")
    {
        _content_length = ft_stoi(value);
        _chunked = false;
    }
    else if (key == "Transfer-Encoding")
    {
        if (value == "chunked")
        {
            _chunked = true;

            // 'Transfer-Encoding: chunked' and 'Content-Length'
            // are not compatible
            if (_content_length > 0)
                throw HTTPMessage::ParseException();
        }
    }
}

void HTTPRequestParser::clear()
{
    _state = PARSE_START;
    _buf.clear();
    _content_length = 0;
    _chunked = false;
}

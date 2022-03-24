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
        _state = rhs._state;
        _buf = rhs._buf;
        _content_length = rhs._content_length;
    }
    return *this;
}

// -----------------------------------------------------------------------------
//                                  Getters
// -----------------------------------------------------------------------------

ParseState HTTPRequestParser::getState() const
{
    return _state;
}

// -----------------------------------------------------------------------------
//                                  Setters
// -----------------------------------------------------------------------------

void HTTPRequestParser::setHeader(const std::string& key, const std::string& value)
{
    HTTPMessage::setHeader(key, value);
    if (key == "Content-Length")
        _content_length = ft_stoi(value);
}

// -----------------------------------------------------------------------------
//                                  Cleaners
// -----------------------------------------------------------------------------

void HTTPRequestParser::clear()
{
    HTTPRequest::clear();
    _state = PARSE_START;
    _buf.clear();
    _content_length = 0;
}

// -----------------------------------------------------------------------------
//                                   Parsers
// -----------------------------------------------------------------------------

ParseState HTTPRequestParser::parse(const std::string& s)
{
    _buf += s;  // append last received chunk to buffer
    if (_state == PARSE_START)
        return _parse_start();
    if (_state == PARSE_HEADER)
        return _parse_headers();
    if (_state == PARSE_CHUNK_HEAD)
        return _parse_chunk_head();
    if (_state == PARSE_CHUNK_CONTENT)
        return _parse_chunk_content();
    if (_state == PARSE_CHUNK_TAIL)
        return _parse_chunk_tail();
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
        {                                       // we are now parsing the body
            if (this->getHeader("Transfer-Encoding") == "chunked")
                _state = PARSE_CHUNK_HEAD;
            else
                _state = PARSE_BODY;
        }
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
    _body += _buf;
    _buf = "";  // clear buffer
    size_t current_body_len = _body.size();
    if (current_body_len > _content_length)
        throw HTTPRequest::ParseException();
    if (current_body_len == _content_length)
        _state = PARSE_COMPLETE;
    return _state;
}

ParseState HTTPRequestParser::_parse_chunk_head()
{
    // check if the body is complete or if we need to consume a new chunk
    size_t pos = _buf.find("\r\n");
    if (pos == std::string::npos)
        return _state;  // fetch more data
    std::string line = _buf.substr(0, pos);
    _buf = _buf.substr(pos + 2);
    std::stringstream ss(line);
    int chunk_size;
    // parse chunk size in hexadecimal format
    ss >> std::hex >> chunk_size;
    if (!ss.eof())
        throw HTTPRequest::ParseException();
    if (chunk_size == 0)
    {
        _content_length = 0;
        _state = PARSE_CHUNK_TAIL;
    }
    else
    {
        _content_length += chunk_size;
        _state = PARSE_CHUNK_CONTENT;
    }
    return this->parse(); // consume buffer
}

ParseState HTTPRequestParser::_parse_chunk_content()
{
    size_t current_body_len = _body.size();
    if (_content_length < current_body_len)
        throw HTTPRequest::ParseException();
    // else
    if (_content_length > current_body_len)
    {
        // consume buffer up to _content_length
        size_t missing_data_len = _content_length - current_body_len;
        if (_buf.size() < missing_data_len)
        {
            _body += _buf;
            _buf = "";
            return _state; // fetch more data
        }
        // else - chunk is complete
        _body += _buf.substr(0, missing_data_len);
        _buf = _buf.substr(missing_data_len);
        _state = PARSE_CHUNK_TAIL;
        return this->parse(); // consume buffer
    }
    _state = PARSE_CHUNK_TAIL;
    return this->parse();
}

ParseState HTTPRequestParser::_parse_chunk_tail()
{
    size_t pos = _buf.find("\r\n");
    if (pos == std::string::npos)
        return _state; // feed more data
    if (pos > 0)
        throw HTTPRequest::ParseException();
    // else (pos == 0)
    _buf = _buf.substr(2);
    if (_content_length == 0)
        _state = PARSE_COMPLETE;
    else
        _state = PARSE_CHUNK_HEAD; // fetch the next chunk
    return this->parse();
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 15:33:45 by jceia             #+#    #+#             */
/*   Updated: 2022/03/20 00:04:30 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "HTTPResponse.hpp"
# include <sstream>
# include <fstream>

HTTPResponse::HTTPResponse() :
    _status_code(0),
    _status_text(""),
    _version("")
{
}

HTTPResponse::~HTTPResponse()
{
}

HTTPResponse& HTTPResponse::operator=(const HTTPResponse &rhs)
{
    if (this != &rhs)
    {
        _status_code = rhs._status_code;
        _status_text = rhs._status_text;
        _version = rhs._version;
    }
    return *this;
}

std::string HTTPResponse::getBody() const
{
    return _body;
}

void HTTPResponse::setStatus(int status_code, const std::string& text)
{
    _status_code = status_code;
    _status_text = text;
}

void HTTPResponse::setHeader(const std::string& name, const std::string& value)
{
    _headers[name] = value;
}

void HTTPResponse::setVersion(const std::string& version)
{
    _version = version;
}

void HTTPResponse::setBody(const std::string& body)
{
    _body = body;
    // set content length
    std::stringstream ss;
    ss << body.size();
    _headers["Content-Length"] = ss.str();
}

void HTTPResponse::setBody(const std::ifstream& f)
{
    std::stringstream ss;
    ss << f.rdbuf();
    this->setBody(ss.str());
}

std::ostream &operator<<(std::ostream &out, const HTTPResponse &response)
{
    out << response._version << " " << response._status_code << " " << response._status_text << "\r\n";
    for (std::map<std::string, std::string>::const_iterator it = response._headers.begin();
        it != response._headers.end(); ++it)
        out << it->first << ": " << it->second << "\r\n";
    out << "\r\n" << response._body;
    return out;
}

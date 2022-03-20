/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 15:33:45 by jceia             #+#    #+#             */
/*   Updated: 2022/03/20 02:12:47 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "HTTPMessage.hpp"
# include "HTTPRequest.hpp"
# include "utils.hpp"
# include <sstream>



HTTPRequest::HTTPRequest() :
    HTTPMessage(),
    _method(GET),
    _endpoint(""),
    _query("")
{
}

HTTPRequest::HTTPRequest(const HTTPRequest& rhs) :
    HTTPMessage(rhs),
    _method(rhs._method),
    _endpoint(rhs._endpoint),
    _query(rhs._query)
{
}

HTTPRequest::~HTTPRequest()
{
}

HTTPRequest& HTTPRequest::operator=(const HTTPRequest &rhs)
{
    HTTPMessage::operator=(rhs);
    if (this != &rhs)
    {
        _method = rhs._method;
        _endpoint = rhs._endpoint;
        _query = rhs._query;
    }
    return *this;
}

std::istream &operator>>(std::istream &is, HTTPRequest &request)
{
    // Start line
    std::string line;
    std::getline(is, line);
    std::stringstream ss(_drop_carriage_return(line));
    std::string method;
    std::string path;
    ss >> method >> path >> request._version;
    request.setPath(path);
    request.setMethod(method);
    if (!ss.eof())
        throw HTTPMessage::ParseException();
    // remaining message
    is >> dynamic_cast<HTTPMessage&>(request);
    return is;
}

std::ostream &operator<<(std::ostream &out, const HTTPRequest &request)
{
    out << request._method << " "
        << request.getPath() << " "
        << request._version << "\r\n"
        << dynamic_cast<const HTTPMessage&>(request);
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

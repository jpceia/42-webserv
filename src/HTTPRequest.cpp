/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 15:33:45 by jceia             #+#    #+#             */
/*   Updated: 2022/03/23 00:56:54 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "HTTPMessage.hpp"
# include "HTTPRequest.hpp"
# include "HTTPMethod.hpp"
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

// -----------------------------------------------------------------------------
//                                   Getters
// -----------------------------------------------------------------------------

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

// -----------------------------------------------------------------------------
//                                   Setters
// -----------------------------------------------------------------------------

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

void HTTPRequest::printStart() const
{
    std::cout << _method << " " << this->getPath() << " " << _version << std::endl;
}

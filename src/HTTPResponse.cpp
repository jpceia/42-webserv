/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 15:33:45 by jceia             #+#    #+#             */
/*   Updated: 2022/03/24 03:34:27 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "HTTPResponse.hpp"
# include "utils.hpp"
# include <sstream>
# include <fstream>


HTTPResponse::HTTPResponse() :
    HTTPMessage(),
    _status()
{
    this->setHeader("Server", "webserv/1.0");
}

HTTPResponse::HTTPResponse(const HTTPResponse& rhs) :
    HTTPMessage(rhs),
    _status(rhs._status)
{
}

HTTPResponse::~HTTPResponse()
{
}

HTTPResponse& HTTPResponse::operator=(const HTTPResponse &rhs)
{
    HTTPMessage::operator=(rhs);
    if (this != &rhs)
        _status = rhs._status;
    return *this;
}

// -----------------------------------------------------------------------------
//                                   Setters
// -----------------------------------------------------------------------------

void HTTPResponse::setStatus(const HTTPStatus& status)
{
    _status = status;
}

void HTTPResponse::setBody(const std::string& body)
{
    HTTPMessage::setBody(body);

    // set content length
    HTTPMessage::setHeader("Content-Length", ft_itos(body.size()));
}

// -----------------------------------------------------------------------------
//                                  Cleaners
// -----------------------------------------------------------------------------

void HTTPResponse::clear()
{
    HTTPMessage::clear();
    this->setHeader("Server", "webserv/1.0");
    _status = HTTPStatus();
}

// -----------------------------------------------------------------------------
//                                  Helpers                                     
// -----------------------------------------------------------------------------

void HTTPResponse::printStart() const
{
    std::cout << _version << " " << _status << std::endl;
}

// -----------------------------------------------------------------------------
//                                IO operators
// -----------------------------------------------------------------------------

std::ostream &operator<<(std::ostream &out, const HTTPResponse &response)
{
    out << response._version << " "
        << response._status << "\r\n"
        << dynamic_cast<const HTTPMessage&>(response);
    return out;
}

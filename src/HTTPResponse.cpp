/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 15:33:45 by jceia             #+#    #+#             */
/*   Updated: 2022/03/23 00:57:20 by jpceia           ###   ########.fr       */
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

void HTTPResponse::setStatus(int status_code, const std::string& text)
{
    _status.code = status_code;
    _status.text = text;
}

void HTTPResponse::setBody(const std::string& body)
{
    HTTPMessage::setBody(body);

    // set content length
    HTTPMessage::setHeader("Content-Length", ft_itos(body.size()));
}

void HTTPResponse::printStart() const
{
    std::cout << _version << " " << _status.code << " " << _status.text << std::endl;
}

std::ostream &operator<<(std::ostream &out, const HTTPStatus& status)
{
    out << status.code << " " << status.text;
    return out;
}

std::ostream &operator<<(std::ostream &out, const HTTPResponse &response)
{
    out << response._version << " "
        << response._status << "\r\n"
        << dynamic_cast<const HTTPMessage&>(response);
    return out;
}

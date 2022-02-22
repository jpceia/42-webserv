/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jceia <jceia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 15:33:45 by jceia             #+#    #+#             */
/*   Updated: 2022/02/22 16:09:55 by jceia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "HTTPResponse.hpp"
# include <sstream>

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
        this->_status_code = rhs._status_code;
        this->_status_text = rhs._status_text;
        this->_version = rhs._version;
    }
    return *this;
}

std::ostream &operator<<(std::ostream &out, HTTPResponse &response)
{
    out << response._version << " " << response._status_code << " " << response._status_text << "\r\n";
    for (std::map<std::string, std::string>::iterator it = response._headers.begin(); it != response._headers.end(); ++it)
        out << it->first << ": " << it->second << "\r\n";
    out << "\r\n" << response._body;
    return out;
}

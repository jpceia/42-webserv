/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPStatus.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 00:12:30 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/24 00:29:12 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPStatus.hpp"
#include <iostream>
#include <map>

// Static members
HTTPStatus::StatusTextMap HTTPStatus::_message = HTTPStatus::initStatusMap();

// Constructors
HTTPStatus::HTTPStatus(int code) :
    _code(code)
{
}

HTTPStatus::HTTPStatus(const HTTPStatus& rhs)
{
    if (this != &rhs)
        _code = rhs._code;
}

HTTPStatus::~HTTPStatus()
{
}

HTTPStatus& HTTPStatus::operator=(const HTTPStatus& rhs)
{
    if (this != &rhs)
        _code = rhs._code;
    return *this;
}

// -----------------------------------------------------------------------------
//                                   Getters
// -----------------------------------------------------------------------------

int HTTPStatus::getCode() const
{
    return _code;
}

// -----------------------------------------------------------------------------
//                         Static members initialization
// -----------------------------------------------------------------------------

HTTPStatus::StatusTextMap HTTPStatus::initStatusMap() {
    StatusTextMap message;

    // 1xx Informational
    message[100] = "Continue";
    message[101] = "Switching Protocols";
    message[102] = "Processing";

    // 2xx Success
    message[200] = "OK";
    message[201] = "Created";
    message[202] = "Accepted";
    message[203] = "Non-Authoritative Information";
    message[204] = "No Content";
    message[205] = "Reset Content";
    message[206] = "Partial Content";
    message[207] = "Multi-Status";
    message[208] = "Already Reported";
    message[226] = "IM Used";

    // 3xx Redirection
    message[300] = "Multiple Choices";
    message[301] = "Moved Permanently";
    message[302] = "Found";
    message[303] = "See Other";
    message[304] = "Not Modified";
    message[305] = "Use Proxy";
    message[306] = "Switch Proxy";
    message[307] = "Temporary Redirect";
    message[308] = "Permanent Redirect";

    // 4xx Client Error
    message[400] = "Bad Request";
    message[401] = "Unauthorized";
    message[402] = "Payment Required";
    message[403] = "Forbidden";
    message[404] = "Not Found";
    message[405] = "Method Not Allowed";
    message[406] = "Not Acceptable";
    message[407] = "Proxy Authentication Required";
    message[408] = "Request Timeout";
    message[409] = "Conflict";
    message[410] = "Gone";
    message[411] = "Length Required";
    message[412] = "Precondition Failed";
    message[413] = "Payload Too Large";
    message[414] = "URI Too Long";
    message[415] = "Unsupported Media Type";
    message[416] = "Range Not Satisfiable";
    message[417] = "Expectation Failed";
    message[418] = "I'm a teapot";
    message[421] = "Misdirected Request";
    message[422] = "Unprocessable Entity";
    message[423] = "Locked";
    message[424] = "Failed Dependency";
    message[426] = "Upgrade Required";
    message[428] = "Precondition Required";
    message[429] = "Too Many Requests";
    message[431] = "Request Header Fields Too Large";
    message[451] = "Unavailable For Legal Reasons";

    // 5xx Server Error
    message[500] = "Internal Server Error";
    message[501] = "Not Implemented";
    message[502] = "Bad Gateway";
    message[503] = "Service Unavailable";
    message[504] = "Gateway Timeout";
    message[505] = "HTTP Version Not Supported";
    message[506] = "Variant Also Negotiates";
    message[507] = "Insufficient Storage";
    message[508] = "Loop Detected";
    message[510] = "Not Extended";
    message[511] = "Network Authentication Required";
    return message;
}

// -----------------------------------------------------------------------------
//                                IO operators
// -----------------------------------------------------------------------------

std::ostream &operator<<(std::ostream &out, const HTTPStatus& status)
{
    HTTPStatus::StatusTextMap::const_iterator it = HTTPStatus::_message.find(status._code);
    out << status._code << " ";
    if (it != HTTPStatus::_message.end())
        out << it->second;
    else
        out << "Unknown status code";
    return out;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 02:17:03 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/26 01:37:19 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPMethod.hpp"
#include "HTTPMessage.hpp"
#include <iostream>


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
    else if (s == "CONNECT")
        method = CONNECT;
    else if (s == "OPTIONS")
        method = OPTIONS;
    else if (s == "TRACE")
        method = TRACE;
    else if (s == "PATCH")
        method = PATCH;
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
    case CONNECT:
        out << "CONNECT";
        break ;
    case OPTIONS:
        out << "OPTIONS";
        break ;
    case TRACE:
        out << "TRACE";
        break ;
    case PATCH:
        out << "PATCH";
        break ;   
    default:
        throw std::runtime_error("Unkown HTTP method");
    }
    return out;
}

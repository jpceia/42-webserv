/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 13:00:01 by jceia             #+#    #+#             */
/*   Updated: 2022/02/23 20:04:47 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <iostream>
# include <map>

// https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages

enum HTTPMethod {
    GET,
    POST,
    DELETE,
    UNKNOWN
};

enum ParseStatus {
    PARSE_START,
    PARSE_HEADER,
    PARSE_BODY,
    PARSE_COMPLETE
};

class HTTPRequest
{
public:
    HTTPRequest();
    virtual ~HTTPRequest();

    HTTPRequest &operator=(const HTTPRequest &rhs);

    ParseStatus parse(const std::string& s);

    friend std::istream &operator>>(std::istream &is, HTTPRequest &request);
    friend std::ostream &operator<<(std::ostream &out, const HTTPRequest &request);

    static HTTPMethod parseMethod(std::string const &s);
    static std::string strMethod(HTTPMethod method);
    class ParseException : public std::exception
    {
        public:
            virtual const char* what(void) const throw();
    };
    
private:
    HTTPMethod _method;
    std::string _path;
    std::string _body;
    std::string _version;
    std::map<std::string, std::string> _headers;

    ParseStatus _parse_status;
    std::string _buf;
};

#endif

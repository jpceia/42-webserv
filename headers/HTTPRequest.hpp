/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jceia <jceia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 13:00:01 by jceia             #+#    #+#             */
/*   Updated: 2022/03/03 17:45:54 by jceia            ###   ########.fr       */
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
    DELETE
};

class HTTPRequest
{
public:
    HTTPRequest();
    virtual ~HTTPRequest();

    HTTPRequest &operator=(const HTTPRequest &rhs);

    friend std::istream &operator>>(std::istream &is, HTTPRequest &request);
    friend std::ostream &operator<<(std::ostream &out, const HTTPRequest &request);

    static HTTPMethod parseMethod(std::string const &s);
    static std::string strMethod(HTTPMethod method);

    void addHeader(const std::string& s);
    virtual void addHeader(const std::string& key, const std::string& value);

    std::string getPath() const { return _path; }

    class ParseException : public std::exception
    {
        public:
            virtual const char* what(void) const throw();
    };
    
protected:
    HTTPMethod _method;
    std::string _path;
    std::string _body;
    std::string _version;
    std::map<std::string, std::string> _headers;
};

#endif

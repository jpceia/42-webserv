/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 13:00:01 by jceia             #+#    #+#             */
/*   Updated: 2022/03/19 15:54:30 by tisantos         ###   ########.fr       */
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
    PUT,
    DELETE,
    HEAD
};

class HTTPRequest
{
public:
    HTTPRequest();
    virtual ~HTTPRequest();

    HTTPRequest &operator=(const HTTPRequest &rhs);

    friend std::istream &operator>>(std::istream &is, HTTPRequest &request);
    friend std::ostream &operator<<(std::ostream &out, const HTTPRequest &request);

    void addHeader(const std::string& s);
    virtual void addHeader(const std::string& key, const std::string& value);

    std::string getVersion() const;
    std::string getPath() const;
    HTTPMethod getMethod() const;
    std::string getHeader(const std::string& key) const;
    std::map<std::string, std::string> getHeaders() const;
    std::string getQueryString() const;
    std::string getEndpoint() const;
    std::string getBody() const;

    void setPath(const std::string& path);
    void setMethod(const std::string& method);

    class ParseException : public std::exception
    {
        public:
            virtual const char* what(void) const throw();
    };
    
protected:
    HTTPMethod _method;
    std::string _endpoint;
    std::string _query;
    std::string _body;
    std::string _version;
    std::map<std::string, std::string> _headers;
};

std::istream &operator>>(std::istream &is, HTTPMethod &method);
std::ostream &operator<<(std::ostream &out, const HTTPMethod &method);

#endif

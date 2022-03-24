/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 13:00:01 by jceia             #+#    #+#             */
/*   Updated: 2022/03/23 23:54:24 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include "HTTPMessage.hpp"
# include "HTTPMethod.hpp"
# include <iostream>
# include <map>

class HTTPRequest : public HTTPMessage
{
public:
    HTTPRequest();
    HTTPRequest(const HTTPRequest& rhs);
    virtual ~HTTPRequest();
    HTTPRequest &operator=(const HTTPRequest &rhs);

    // Getters
    std::string getPath() const;
    HTTPMethod getMethod() const;
    std::string getQueryString() const;
    std::string getEndpoint() const;

    // Setters
    void setPath(const std::string& path);
    void setMethod(const std::string& method);

    // Clear
    virtual void clear();

    // Helpers
    void printStart() const;

    // IO operators
    friend std::istream &operator>>(std::istream &is, HTTPRequest &request);
    friend std::ostream &operator<<(std::ostream &out, const HTTPRequest &request);
    
protected:
    HTTPMethod _method;
    std::string _endpoint;
    std::string _query;
};

#endif

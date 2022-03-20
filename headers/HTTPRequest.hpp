/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 13:00:01 by jceia             #+#    #+#             */
/*   Updated: 2022/03/20 02:16:25 by jpceia           ###   ########.fr       */
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

    friend std::istream &operator>>(std::istream &is, HTTPRequest &request);
    friend std::ostream &operator<<(std::ostream &out, const HTTPRequest &request);

    std::string getPath() const;
    HTTPMethod getMethod() const;
    std::string getQueryString() const;
    std::string getEndpoint() const;

    void setPath(const std::string& path);
    void setMethod(const std::string& method);
    
protected:
    HTTPMethod _method;
    std::string _endpoint;
    std::string _query;
};

#endif

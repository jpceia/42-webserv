/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jceia <jceia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 13:00:01 by jceia             #+#    #+#             */
/*   Updated: 2022/02/22 15:37:50 by jceia            ###   ########.fr       */
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
    UNKNOWN
};

class HTTPRequest
{

public:
    HTTPRequest();
    ~HTTPRequest();

    HTTPRequest &operator=(const HTTPRequest &rhs);

    friend std::istream &operator>>(std::istream &is, HTTPRequest &request);
    friend std::ostream &operator<<(std::ostream &out, HTTPRequest &request);
private:
    HTTPMethod _method;
    std::string _path;
    std::string _body;
    std::string _version;
    std::map<std::string, std::string> _headers;
};

#endif

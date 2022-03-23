/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 15:40:09 by jceia             #+#    #+#             */
/*   Updated: 2022/03/23 00:52:47 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include "HTTPMessage.hpp"
# include <iostream>
# include <map>

struct HTTPStatus
{
public:
    HTTPStatus() :
        code(200),
        text("OK")
    {}

    int code;
    std::string text;
};

std::ostream &operator<<(std::ostream &out, const HTTPStatus& status);

class HTTPResponse : public HTTPMessage
{

public:
    HTTPResponse();
    HTTPResponse(const HTTPResponse& rhs);
    ~HTTPResponse();
    HTTPResponse &operator=(const HTTPResponse &rhs);

    friend std::ostream &operator<<(std::ostream &out, const HTTPResponse &request);

    void setStatus(int status_code, const std::string& text);
    void setBody(const std::string& body);
    using HTTPMessage::setBody;

    void printStart() const;

private:
    HTTPStatus _status;
};


#endif

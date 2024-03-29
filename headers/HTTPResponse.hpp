/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 15:40:09 by jceia             #+#    #+#             */
/*   Updated: 2022/03/24 00:30:00 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include "HTTPStatus.hpp"
# include "HTTPMessage.hpp"
# include <iostream>
# include <map>

class HTTPResponse : public HTTPMessage
{

public:
    HTTPResponse();
    HTTPResponse(const HTTPResponse& rhs);
    virtual ~HTTPResponse();
    HTTPResponse &operator=(const HTTPResponse &rhs);

    // Setters
    void setStatus(const HTTPStatus& status);
    void setBody(const std::string& body);
    using HTTPMessage::setBody;

    // Clear
    virtual void clear();

    // Helpers
    void printStart() const;

    // IO operators
    friend std::ostream &operator<<(std::ostream &out, const HTTPResponse &request);

private:
    HTTPStatus _status;
};


#endif

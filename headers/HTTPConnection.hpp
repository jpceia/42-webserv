/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPConnection.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 05:15:40 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/09 20:21:35 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCONNECTION_HPP
# define HTTPCONNECTION_HPP

# include "TCPConnection.hpp"
# include "HTTPRequest.hpp"
# include "HTTPResponse.hpp"
# include <sstream>

// https://stackoverflow.com/questions/68468641/parse-http-responses-from-a-tcp-stream

class TCPConnection;

class HTTPConnection : public TCPConnection
{
public:
    HTTPConnection(int fd);
    HTTPConnection(const TCPConnection& rhs);
    HTTPConnection(const HTTPConnection& rhs);

    virtual ~HTTPConnection();
    virtual HTTPConnection& operator=(const HTTPConnection& rhs);

    virtual void sendResponse(const HTTPResponse& response) const;
    virtual HTTPRequest recvRequest() const;
};

#endif

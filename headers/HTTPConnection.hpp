/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPConnection.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 05:15:40 by jpceia            #+#    #+#             */
/*   Updated: 2022/02/23 05:44:19 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCONNECTION_HPP
# define HTTPCONNECTION_HPP

# include "TCPConnection.hpp"
# include "HTTPRequest.hpp"
# include "HTTPResponse.hpp"
# include <sstream>

class TCPConnection;

class HTTPConnection : public TCPConnection
{
public:
    HTTPConnection(int fd) : TCPConnection(fd) {};
    HTTPConnection(const TCPConnection& rhs) : TCPConnection(rhs) {};
    HTTPConnection(const HTTPConnection& rhs) : TCPConnection(rhs) {};

    ~HTTPConnection() {};
    HTTPConnection& operator=(const HTTPConnection& rhs) {
        TCPConnection::operator=(rhs);
        return *this;
    };

    void send(const HTTPResponse& response)
    {
        std::stringstream ss;
        ss << response;
        TCPConnection::send(ss.str());
    }

    HTTPRequest recv()
    {
        std::string msg = TCPConnection::recv();
        std::stringstream ss(msg);

        HTTPRequest request;
        ss >> request;
        return request;
    }
};

#endif

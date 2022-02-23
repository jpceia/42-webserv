/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPConnection.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 05:15:40 by jpceia            #+#    #+#             */
/*   Updated: 2022/02/23 06:06:11 by jpceia           ###   ########.fr       */
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
    HTTPConnection(int fd);
    HTTPConnection(const TCPConnection& rhs);
    HTTPConnection(const HTTPConnection& rhs);

    ~HTTPConnection();
    HTTPConnection& operator=(const HTTPConnection& rhs);

    void send(const HTTPResponse& response);
    HTTPRequest recv();
};

#endif

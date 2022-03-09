/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPListener.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 02:08:39 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/09 16:35:38 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPConnection.hpp"
#include "HTTPListener.hpp"
#include "TCPListener.hpp"

HTTPListener::HTTPListener(int sock) :
    TCPListener(sock)
{
}

HTTPListener::HTTPListener(const std::string& host, int port) :
    TCPListener(host, port)
{
}

HTTPListener::HTTPListener(const HTTPListener& rhs) :
    TCPListener(rhs)
{
}

HTTPListener::~HTTPListener()
{
}

HTTPListener& HTTPListener::operator=(const HTTPListener& rhs)
{
    TCPListener::operator=(rhs);
    return *this;
}

TCPConnection* HTTPListener::accept() const
{
    return new HTTPConnection(_accept());
}

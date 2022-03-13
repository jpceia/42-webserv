/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPListener.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 02:08:39 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/09 18:05:54 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPConnection.hpp"
#include "HTTPStatefulConnection.hpp"
#include "HTTPListener.hpp"
#include "TCPListener.hpp"
#include "configFile.hpp"

HTTPListener::HTTPListener(int sock) :
    TCPListener(sock)
{
}

HTTPListener::HTTPListener(const std::string& host, int port, const std::vector<configServerBlock>& configs) :
    TCPListener(host, port), _configs(configs)
{
}

HTTPListener::HTTPListener(const HTTPListener& rhs) :
    TCPListener(rhs), _configs(rhs._configs)
{
}

HTTPListener::~HTTPListener()
{
}

HTTPListener& HTTPListener::operator=(const HTTPListener& rhs)
{
    TCPListener::operator=(rhs);
    _configs = rhs._configs;
    return *this;
}

TCPConnection* HTTPListener::accept() const
{
    return new HTTPStatefulConnection(_accept());
}

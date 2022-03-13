/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPListener.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 02:08:39 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/09 16:36:03 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPConnection.hpp"
#include "TCPListener.hpp"

class HTTPListener : public TCPListener
{
public:
    HTTPListener(int sock);
    HTTPListener(const std::string& host, int port);
    HTTPListener(const HTTPListener& rhs);
    virtual ~HTTPListener();
    HTTPListener& operator=(const HTTPListener& rhs);
    
    virtual TCPConnection* accept() const;
};

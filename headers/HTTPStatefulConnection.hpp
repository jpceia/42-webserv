/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPStatefulConnection.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 00:17:07 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/09 20:26:00 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPConnection.hpp"
#include "HTTPRequestParser.hpp"

class HTTPStatefulConnection : public HTTPConnection
{
public:
    HTTPStatefulConnection(int fd);
    HTTPStatefulConnection(const TCPConnectionArgs& args);
    HTTPStatefulConnection(const HTTPConnection& rhs);
    HTTPStatefulConnection(const HTTPStatefulConnection& rhs);
    ~HTTPStatefulConnection();
    HTTPStatefulConnection& operator=(const HTTPStatefulConnection& rhs);
    HTTPRequest getRequest() const;
    void setResponse(const HTTPResponse& response);
    bool sendChuck();
    bool recvChunk();
    void clear();

private:
    HTTPRequestParser _request;
    std::string _response;
};

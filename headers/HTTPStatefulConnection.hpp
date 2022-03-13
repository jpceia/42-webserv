/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPStatefulConnection.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 00:17:07 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/13 17:54:01 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSTATEFULCONNECTION_CPP
# define HTTPSTATEFULCONNECTION_CPP

# include "HTTPConnection.hpp"
# include "HTTPRequestParser.hpp"
# include "configFile.hpp" 

class HTTPStatefulConnection : public HTTPConnection
{
public:
    HTTPStatefulConnection(int fd);
    HTTPStatefulConnection(const TCPConnectionArgs& args, const std::vector<configServerBlock>& configs);
    HTTPStatefulConnection(const HTTPStatefulConnection& rhs);
    ~HTTPStatefulConnection();
    HTTPStatefulConnection& operator=(const HTTPStatefulConnection& rhs);
    HTTPRequest getRequest() const;
    configServerBlock getServerBlock(const std::string& host) const;
    void setResponse(const HTTPResponse& response);
    bool sendChuck();
    bool recvChunk();
    void clear();

private:
    HTTPRequestParser _request;
    std::string _response;
    std::vector<configServerBlock> _configs;
};

#endif

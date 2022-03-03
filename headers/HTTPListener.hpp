/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPListener.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 17:30:40 by jceia             #+#    #+#             */
/*   Updated: 2022/03/03 22:42:00 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPLISTENER_CPP
# define HTTPLISTENER_CPP

# include "TCPListener.hpp"
# include "HTTPRequest.hpp"
# include "HTTPResponse.hpp"
# include "HTTPConnection.hpp"
# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>

class HTTPListener : public TCPListener
{
public:
    HTTPListener(const std::string& host, int port, int timeout);
    virtual ~HTTPListener();

protected:
    void _handle_client_request(int fd);

private:
    HTTPResponse _not_found_response();
    HTTPResponse _build_response(const HTTPRequest& request);

    std::string _root;
    std::vector<std::string> _index;
};

#endif

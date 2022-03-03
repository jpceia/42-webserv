/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPListener.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jceia <jceia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 17:30:40 by jceia             #+#    #+#             */
/*   Updated: 2022/03/03 18:45:02 by jceia            ###   ########.fr       */
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
    std::string _root;
    std::vector<std::string> _index;
};

#endif

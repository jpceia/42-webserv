/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPConnection.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 03:04:11 by jpceia            #+#    #+#             */
/*   Updated: 2022/02/23 03:54:18 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPCONNECTION_HPP
# define TCPCONNECTION_HPP

# include <iostream>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <unistd.h>

# define BUFF_SIZE 1024

class TCPConnection
{
public:
    TCPConnection(int fd);
    TCPConnection(const TCPConnection& rhs);

    ~TCPConnection();
    TCPConnection& operator=(const TCPConnection& rhs);

    void send(const std::string& msg);
    std::string recv();

private:
    int _fd;
};

#endif

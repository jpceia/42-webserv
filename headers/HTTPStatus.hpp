/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPStatus.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 00:10:54 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/24 00:28:19 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSTATUS_HPP
# define HTTPSTATUS_HPP

# include <map>
# include <iostream>

class HTTPStatus
{
public:
    // Member Types
    typedef std::map<int, std::string> StatusTextMap;

    HTTPStatus(int code = 200);
    HTTPStatus(const HTTPStatus& rhs);
    virtual ~HTTPStatus();
    HTTPStatus &operator=(const HTTPStatus &rhs);

    // Getters
    int getCode() const;

    // IO operators
    friend std::ostream &operator<<(std::ostream &out, const HTTPStatus& status);

private:
    // Static members
    static StatusTextMap _message;
    static StatusTextMap initStatusMap();

    // Private members
    int _code;
};

// IO operators
std::ostream &operator<<(std::ostream &out, const HTTPStatus& status);

#endif

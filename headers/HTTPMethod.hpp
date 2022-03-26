/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPMethod.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 02:16:05 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/26 01:34:36 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPMETHOD_HPP
# define HTTPMETHOD_HPP

# include <iostream>

enum HTTPMethod {
    GET,
    POST,
    PUT,
    DELETE,
    HEAD,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH
};

std::istream &operator>>(std::istream &is, HTTPMethod &method);
std::ostream &operator<<(std::ostream &out, const HTTPMethod &method);

#endif

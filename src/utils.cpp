/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 20:06:03 by jpceia            #+#    #+#             */
/*   Updated: 2022/02/24 04:51:40 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include <sstream>

# if __cplusplus < 201103L

int stoi(const std::string& str)
{
    std::stringstream ss(str);
    int i;
    ss >> i;
    return i;
}

# endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 20:06:03 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/03 22:29:23 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>

int ft_stoi(const std::string& str)
{
    std::stringstream ss(str);
    int i;
    ss >> i;
    return i;
}

bool is_dir(const std::string& path)
{
    struct stat s;
    return (stat(path.c_str(), &s) == 0 && S_ISDIR(s.st_mode));
}

bool is_readable_file(const std::string& path)
{
    struct stat s;
    return (stat(path.c_str(), &s) == 0 && S_ISREG(s.st_mode) &&
            (s.st_mode & S_IRUSR));
}

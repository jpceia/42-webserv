/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 20:05:23 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/03 22:30:48 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>

int ft_stoi(const std::string& str);
bool is_dir(const std::string& path);
bool is_readable_file(const std::string& path);


#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 20:05:23 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/04 13:04:16 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <map>
# include <vector>
# include <string>

int ft_stoi(const std::string& str);
bool is_dir(const std::string& path);
bool is_readable_file(const std::string& path);

std::string exec_cmd(
    const std::string &cmd,
    const std::vector<std::string>& args,
    const std::map<std::string, std::string>& env);

#endif

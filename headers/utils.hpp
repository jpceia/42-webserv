/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 20:05:23 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/24 07:27:37 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <map>
# include <vector>
# include <string>
# include <sys/poll.h>

bool compareFunction (std::string a, std::string b);
int ft_stoi(const std::string& str);
std::string ft_itos(int i);
bool is_dir(const std::string& path);
bool is_readable_file(const std::string& path);

struct pollfd create_pollfd(int fd, short int events);

std::string cleanup_path(const std::string& path);

std::string exec_cmd(
    const std::string &cmd,
    const std::vector<std::string>& args,
    const std::map<std::string, std::string>& env,
    const std::string& input);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 20:05:23 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/19 17:53:36 by tisantos         ###   ########.fr       */
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
bool is_dir(const std::string& path);
bool is_readable_file(const std::string& path);

struct pollfd create_pollfd(int fd, short int events);

std::string exec_cmd(
    const std::string &cmd,
    const std::vector<std::string>& args,
    const std::map<std::string, std::string>& env);

#endif

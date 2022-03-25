/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 15:58:22 by tisantos          #+#    #+#             */
/*   Updated: 2022/03/19 16:16:17 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_HPP
# define DEBUG_HPP

# include <iostream>
# include "HTTPServer.hpp"
# include "HTTPRequest.hpp"

void debug_request(const HTTPRequest& request);
void debug_context(const struct Context& ctx);

#endif

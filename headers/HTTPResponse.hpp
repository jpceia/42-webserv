/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jceia <jceia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 15:40:09 by jceia             #+#    #+#             */
/*   Updated: 2022/02/22 16:09:38 by jceia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <iostream>
# include <map>

class HTTPResponse
{

public:
    HTTPResponse();
    ~HTTPResponse();

    HTTPResponse &operator=(const HTTPResponse &rhs);

    friend std::ostream &operator<<(std::ostream &out, HTTPResponse &request);

private:
    int _status_code;
    std::string _status_text;
    std::string _version;
    std::string _body;
    std::map<std::string, std::string> _headers;
};


#endif

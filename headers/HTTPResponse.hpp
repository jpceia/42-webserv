/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 15:40:09 by jceia             #+#    #+#             */
/*   Updated: 2022/02/23 05:45:00 by jpceia           ###   ########.fr       */
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

    friend std::ostream &operator<<(std::ostream &out, const HTTPResponse &request);

    void setStatus(int status_code, const std::string& text);
    void setHeader(const std::string& name, const std::string& value);
    void setVersion(const std::string& version);
    void setBody(const std::string& body);
    void setBody(const std::ifstream& f);

private:
    int _status_code;
    std::string _status_text;
    std::string _version;
    std::string _body;
    std::map<std::string, std::string> _headers;
};


#endif

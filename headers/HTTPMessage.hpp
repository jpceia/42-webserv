/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPMessage.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 00:41:19 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/20 01:30:52 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPMESSAGE_HPP
# define HTTPMESSAGE_HPP

# include <iostream>
# include <map>

class HTTPMessage
{
public:
    HTTPMessage();
    HTTPMessage(const HTTPMessage &rhs);
    virtual ~HTTPMessage();
    HTTPMessage& operator=(const HTTPMessage &rhs);

    // Getters
    std::string getVersion() const;
    std::map<std::string, std::string> getHeaders() const;
    std::string getHeader(const std::string& key) const;
    std::string getBody() const;

    // Setters
    virtual void setVersion(const std::string& version);
    virtual void setHeader(const std::string& key, const std::string& value);
    virtual void setHeader(const std::string& line);
    virtual void setBody(const std::string& body);
    virtual void setBody(const std::ifstream& f);

    // io
    friend std::istream &operator>>(std::istream &is, HTTPMessage &msg);
    friend std::ostream &operator<<(std::ostream &out, const HTTPMessage &msg);

    // Exceptions
    class ParseException : public std::exception
    {
        public:
            virtual const char* what(void) const throw();
    };

protected:

    // private attributes
    std::string _version;
    std::string _body;
    std::map<std::string, std::string> _headers;
};

// Helpers
std::string& _drop_carriage_return(std::string& s);

#endif

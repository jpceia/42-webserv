/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPMessage.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 00:41:19 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/24 02:16:29 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPMESSAGE_HPP
# define HTTPMESSAGE_HPP

# include <iostream>
# include <map>

/************************************************************************/
/* Comparator for case-insensitive comparison in STL assos. containers  */
/************************************************************************/

// https://stackoverflow.com/questions/1801892/how-can-i-make-the-mapfind-operation-case-insensitive
struct ci_less : std::binary_function<std::string, std::string, bool>
{
    // case-independent (ci) compare_less binary function
    struct nocase_compare : public std::binary_function<unsigned char,unsigned char,bool> 
    {
        bool operator() (const unsigned char& c1, const unsigned char& c2) const
        {
            return tolower (c1) < tolower (c2); 
        }
    };

    bool operator() (const std::string & s1, const std::string & s2) const
    {
        return std::lexicographical_compare 
        (s1.begin (), s1.end (),   // source range
        s2.begin (), s2.end (),   // dest range
        nocase_compare ());  // comparison
    }
};

// https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages

typedef std::map<std::string, std::string, ci_less> HeaderMap;

class HTTPMessage
{
public:
    HTTPMessage();
    HTTPMessage(const HTTPMessage &rhs);
    virtual ~HTTPMessage();
    HTTPMessage& operator=(const HTTPMessage &rhs);

    // Getters
    std::string getVersion() const;
    HeaderMap getHeaders() const;
    std::string getHeader(const std::string& key) const;
    std::string getBody() const;

    // Setters
    virtual void setVersion(const std::string& version);
    virtual void setHeader(const std::string& key, const std::string& value);
    virtual void setHeader(const std::string& line);
    virtual void setBody(const std::string& body);
    virtual void setBody(const std::ifstream& f);

    // Remover
    virtual void removeHeader(const std::string& key);

    // Clear
    virtual void clear();

    // Helpers
    virtual void printStart() const;
    virtual void printHeaders() const;
    virtual void printBody() const;
    virtual void print() const;

    // IO operators
    friend std::istream &operator>>(std::istream &is, HTTPMessage &msg);
    friend std::ostream &operator<<(std::ostream &out, const HTTPMessage &msg);

    // Custom exceptions
    class ParseException : public std::exception
    {
        public:
            virtual const char* what(void) const throw();
    };

protected:

    // private attributes
    std::string _version;
    std::string _body;
    HeaderMap _headers;
};

// Helpers
std::string& _drop_carriage_return(std::string& s, bool raise = true);

#endif

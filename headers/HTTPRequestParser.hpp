/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 05:06:57 by jpceia            #+#    #+#             */
/*   Updated: 2022/02/24 05:32:03 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTPARSE_HPP
# define HTTPREQUESTPARSE_HPP

# include "HTTPRequest.hpp"

enum ParseState {
    PARSE_START,
    PARSE_HEADER,
    PARSE_BODY,
    PARSE_COMPLETE
};

class HTTPRequestParser : public HTTPRequest
{
public:
    HTTPRequestParser();
    HTTPRequestParser(const HTTPRequestParser& rhs);
    virtual ~HTTPRequestParser();

    HTTPRequestParser& operator=(const HTTPRequestParser& rhs);

    ParseState getState() const;

    ParseState parse(const std::string& s, bool new_chunk=true);
    void addHeader(const std::string& line);
    void addHeader(const std::string& key, const std::string& value);

    void clear();

private:
    ParseState _state;
    std::string _buf;
    size_t _content_length;
    bool _by_chunks;
};

#endif

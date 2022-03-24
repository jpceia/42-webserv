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
    PARSE_CHUNK_HEAD,
    PARSE_CHUNK_CONTENT,
    PARSE_CHUNK_TAIL,
    PARSE_COMPLETE
};

class HTTPRequestParser : public HTTPRequest
{
public:
    HTTPRequestParser();
    HTTPRequestParser(const HTTPRequestParser& rhs);
    virtual ~HTTPRequestParser();
    HTTPRequestParser& operator=(const HTTPRequestParser& rhs);

    // Getters
    ParseState getState() const;

    // Setters
    void setHeader(const std::string& key, const std::string& value);

    // Clear
    void clear();

    // Parsers
    ParseState parse(const std::string& s = "");

private:
    ParseState _parse_start();
    ParseState _parse_headers();
    ParseState _parse_body();
    ParseState _parse_chunk_head();
    ParseState _parse_chunk_content();
    ParseState _parse_chunk_tail();

    ParseState _state;
    std::string _buf;
    size_t _content_length;
    bool _chunked;
};

#endif

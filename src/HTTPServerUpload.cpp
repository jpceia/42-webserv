/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServerUpload.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 04:14:47 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/25 15:29:49 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPServer.hpp"
#include "utils.hpp"

// https://stackoverflow.com/questions/8659808/how-does-http-file-upload-work


HTTPResponse HTTPServer::_upload_response(const HTTPRequest& request, const Context& ctx) const
{
    HTTPMethod method = request.getMethod();
    if (method == DELETE)
        return _delete_response(ctx);
    if (method == GET)
        return _static_response(ctx);
    if (method != POST && method != PUT)
        return _status_page_response(405, ctx);

    std::string content_type = request.getHeader("Content-Type");
    if (content_type == "" || content_type.find("text/html") != std::string::npos)
        return _upload_raw_response(request, ctx);
    if (content_type.find("multipart/form-data") != std::string::npos)
        return _upload_multipart_response(request, ctx);
    return _status_page_response(415, ctx);
}

HTTPResponse HTTPServer::_upload_raw_response(const HTTPRequest& request, const Context& ctx) const
{
    HTTPResponse response;
    response.setHeader("Content-Type", "text/html");
    // write to file
    std::ofstream ofs(ctx.getSysPath().c_str(), std::ios::binary | std::ios::trunc);
    if (!ofs.good())
        return _status_page_response(500, ctx);
    std::string body = request.getBody();
    ofs.write(body.c_str(), body.size());
    ofs.close();

    response.setBody("Upload Successfull");
    return response;
}

HTTPResponse HTTPServer::_upload_multipart_response(const HTTPRequest& request, const Context& ctx) const
{
    std::string content_type = request.getHeader("Content-Type");
    size_t pos = content_type.find("boundary=");
    if (pos == std::string::npos)
        return _status_page_response(400, ctx);
    std::string boundary = content_type.substr(pos + 9);

    // Parse the request body using the boundary
    std::stringstream body(request.getBody());
    std::string line;
    std::string chunk;
    while (std::getline(body, line))
    {
        if (line.find(boundary) != std::string::npos) // end of chunk
        {
            _handle_multipart_chunk(chunk, ctx);
            chunk = "";
        }
        else
            chunk += line + "\n";
    }
    // Final response after successfull upload
    HTTPResponse response;
    response.setHeader("Content-Type", "text/html");
    response.setBody("Upload Successfull");
    return response;
}

void HTTPServer::_handle_multipart_chunk(const std::string& chunk, const Context& ctx) const
{
    HTTPMessage msg;
    {
        // Parse the chunk
        std::stringstream ss(chunk);
        ss >> msg;
    }
    std::string content_disposition = msg.getHeader("Content-Disposition");
    if (content_disposition == "")
        return ;
    // get filename
    size_t pos = content_disposition.find("filename=");
    if (pos == std::string::npos)
        return ;
    std::string filename = content_disposition.substr(pos + 10);
    filename = filename.substr(0, filename.find("\""));

    // write to file
    std::string path = cleanup_path(ctx.getSysPath() + "/" + filename);
    std::cout << "Upload path: " << path << std::endl;
    std::ofstream ofs(path.c_str(), std::ios::binary | std::ios::trunc);
    if (!ofs.good())
        return ;
    std::stringstream ss(msg.getBody());
    ofs << ss.rdbuf();
    ofs.close();
}

HTTPResponse HTTPServer::_delete_response(const Context& ctx) const
{
    if (std::remove(ctx.getSysPath().c_str()) != 0)
        return _status_page_response(500, ctx);

    HTTPResponse response = _status_page_response(202, ctx);
    response.setBody("Delete Successfull");
    return response;
}

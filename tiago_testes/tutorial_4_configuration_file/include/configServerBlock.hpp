#ifndef CONFIGSERVERBLOCK_HPP
# define CONFIGSERVERBLOCK_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <string>

class serverBlock
{
    public:
        serverBlock()
        {};

        serverBlock(std::string configuration_file_raw)
        {

        };

        ~serverBlock()
        {};

        void    fillServerBlocks()
        {};

        /********************/
        /* Helper Functions */
        /********************/

        void    fillDefaultConfigurations()
        {
            _listen_default.push_back("0.0.0.0:80");
            _server_name_default.push_back("");
            _error_page_default.push_back("");
            _client_max_body_size_default.push_back("1m");
            _root_default.push_back("./www/");            // These needs to be checked
            _index_default.push_back("./www/index.html"); // These needs to be checked
            _auto_index_default.push_back("off");

            _methods_default.push_back("GET");
            _redirect_default.push_back("");
            _cgi_default.push_back("");
            _upload_default.push_back("off");
        };


    private:
        /********/
        /* Data */
        /********/

        /******************/
        /* Configurations */
        /******************/
        std::vector<std::string>    _listen_default;
        std::vector<std::string>    _server_name_default;
        std::vector<std::string>    _error_page_default;
        std::vector<std::string>    _client_max_body_size_default;
        std::vector<std::string>    _root_default;
        std::vector<std::string>    _index_default;
        std::vector<std::string>    _auto_index_default;

        std::vector<std::string>    _methods_default;
        std::vector<std::string>    _redirect_default;
        std::vector<std::string>    _cgi_default;
        std::vector<std::string>    _upload_default;
};

#endif

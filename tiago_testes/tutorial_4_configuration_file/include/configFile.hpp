#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "serverBlock.hpp"

class configFile
{
    public:
        /****************/
        /* Constructors */
        /****************/
        configFile(char *configuration_file) 
        {
            /*****************************************************************/
            /* Pass the configuration file to string _configuration_file_raw */
            /*****************************************************************/
            std::string line;
            std::ifstream config_file(configuration_file); 
            if (config_file.is_open())
            {
                while (getline(config_file, line))
                {
                    _configuration_file_raw += line;
                    _configuration_file_raw += "\n";
                }
                config_file.close();
            }
            else
            {                
                std::cout << "Configuration File not found or Invalid (Except Error Here) !" << std::endl;
            }

            /***********************************/
            /* Fill all default configurations */
            /***********************************/
            fillDefaultConfigurations();

            /*************************************************************************/
            /* Fill _server_blocks.                                                  */
            /* Server blocks objects will have all the server_blocks already filled. */
            /* With their listen, server_name, location... all filled.               */ 
            /*************************************************************************/
            serverBlock server_block(_configuration_file_raw);
            server_block.fillServerBlocks();

        };
        ~configFile()
        {};

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

        /********************/
        /* Helper Functions */
        /********************/
        void    printConfigurationFileRawException()
        {
            std::cout << _configuration_file_raw;
        };

    private:
        /********/
        /* Data */
        /********/
        std::string                 _configuration_file_raw;

        /**********/
        /* Blocks */
        /**********/
        std::vector<serverBlock>    _server_blocks;

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

/*
server 
{
	listen 127.0.0.1:8080;
	server_name localhost;
}
*/
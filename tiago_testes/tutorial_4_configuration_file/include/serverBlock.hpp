#pragma once

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
            treatConfigurationFile(configuration_file_raw);
        };

        ~serverBlock()
        {};

        void    treatConfigurationFile(std::string configuration_file_raw)
        {
            /******************************/
            /* First step:                */
            /*  Remove all spaces / tabs. */
            /******************************/   
            std::string result;

            std::istringstream iss(configuration_file_raw);
            
            for (std::string line; std::getline(iss, line, ' ');)
            {
                for (std::string line_2; std::getline(iss, line_2, '\t');)
                {
                    result += line_2;                    
                }
                _configuration_file_treated.push_back(result);
            }

            printConfigurationFileTreated();
        }

        void    fillServerBlocks()
        {

        };

        /********************/
        /* Helper Functions */
        /********************/
        void    printConfigurationFileTreated()
        {
            std::vector<std::string>::iterator it(_configuration_file_treated.begin());
            for (int i = 0; it != _configuration_file_treated.end(); it++)
            { 
                std::cout << i  << " = " << *it << std::endl;
                i++;
            }
        };


    private:
        /********/
        /* Data */
        /********/
        std::vector<std::string>                 _configuration_file_treated;

};
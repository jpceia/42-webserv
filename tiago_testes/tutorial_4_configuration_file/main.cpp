#include <iostream>
#include "./include/configFile.hpp"

int main(int argc, char **argv)
{
    std::string line;
    if (argc != 2)
    {
        std::cout << "Insert the Configuration File" << std::endl;
        return (1);
    }


    configFile  configObj(argv[1]);    
    //configObj.printConfigurationFileRawException();   

 
}
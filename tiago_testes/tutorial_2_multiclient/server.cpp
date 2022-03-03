#include "includes/MultiClientChat.hpp"
#include "includes/WebServer.hpp"

int ft_stoi(const std::string& str)
{
    std::stringstream ss(str);
    int i;
    ss >> i;
    return i;
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "./webserv [web|chat] [port]" << std::endl;
        return (0);
    }
    
    int port;
    port = ft_stoi(argv[2]);
    
    if (!strcmp(argv[1],"web"))
    {
	    WebServer		web_server("0.0.0.0", port);

        if (web_server.init(-1) != 0) {
            return 1;
        }

        web_server.run();
    }
    else if (!strcmp(argv[1],"chat")) 
    {
	    MultiClientChat		chat_server("0.0.0.0", port);

        if (chat_server.init(-1) != 0) {
            return 1;
        }

        chat_server.run();
    }
    else 
    {
        std::cout << "You need to specify [web] or [port]" << std::endl;
    }
}

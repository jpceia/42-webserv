#include "includes/MultiClientChat.hpp"

int ft_stoi(const std::string& str)
{
    std::stringstream ss(str);
    int i;
    ss >> i;
    return i;
}

int main(int argc, char **argv) {

    int PORT = 54000;

    if (argc == 2) 
    {
        PORT = ft_stoi(argv[1]);
    }

	MultiClientChat		multi_client_chat("0.0.0.0", PORT);

	if (multi_client_chat.init(-1) != 0) {
		return 1;
	}

	multi_client_chat.run();

	/*while (1)
	{};*/
}

// https://gist.github.com/codehoose/d7dea7010d041d52fb0f59cbe3826036

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

#define PORT 54000

int main()
{
    //	Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        return 1;
    }

    //	Create a hint structure for the server we're connecting with
    std::string ipAddress = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(PORT);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    //	Connect to the server on the socket
    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1)
    {
        return 1;
    }



    do {
		//		Response from server
		char buf[4096];
		std::string userInput;
		memset(buf, 0, 4096);
		int bytesReceived = recv(sock, buf, 4096, 0);
		if (bytesReceived == -1)
		{
			std::cout << "There was an error getting response from server\r\n";
		}
		else
		{
			//		Display response
			std::cout << "SERVER: " << std::string(buf, bytesReceived);// << "\r\n";
		}
        //		Enter lines of text
        std::cout << "> ";
        getline(std::cin, userInput);

        //		Send to server
        int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0);
        if (sendRes == -1)
        {
            std::cout << "Could not send to server! Whoops!\r\n";
            continue;
        }
		else
		{
			std::cout << " message sent!\n";
		}

    } while(true);

    //	Close the socket
    close(sock);

    return 0;
}

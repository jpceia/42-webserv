#pragma once

#include "TcpListener.hpp"

class WebServer : public TcpListener
{
	public:

		WebServer(const char* ipAddress, int port) :	TcpListener(ipAddress, port)
		{};

	protected:

		/*******************************************/
		/* Handler for client connections.		   */
		/*******************************************/
		void	onClientConnected(int clientSocket)
		{
			// Send a welcome message to the connected client
			std::string welcomeMsg = "Welcome to the chat!\r\n";
			sendToClient(clientSocket, welcomeMsg.c_str(), welcomeMsg.size() + 1);
		};

		/*******************************************/
		/* Handler for client disconnections.	   */
		/*******************************************/
		void	onClientDisconnected(int clientSocket)
		{};

		/*********************************************/
		/* Handler for when the message is received  */
		/* from the client.                          */
		/*********************************************/
		int 	onMessageReceived(int clientSocket, const char *msg, int length)
		{
            // GET /index.html HTTP/1.1

            // Parse out the document requested
            // Open the document in the local file system
            // Write the document back to the client
            

            // Write the document back to the client
            std::ostringstream oss;
            oss << "HTTP/1.1 200 OK\r\n";
            oss << "Cache-Control: no-cache, private\r\n";
            oss << "Content-Type: text/html\r\n";
            oss << "Content-Length: 5\r\n";
            oss << "\r\n";
            oss << "hello";

            std::string output = oss.str();
            int size = output.size() + 1;

            int send_ret = sendToClient(clientSocket, output.c_str(), size);
            std::cout << "SEND = " << send_ret << std::endl;

            return (send_ret);
		};

	private:

};

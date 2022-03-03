#pragma once

#include "TcpListener.hpp"


class MultiClientChat : public TcpListener
{
	public:

		MultiClientChat(const char* ipAddress, int port) :	TcpListener(ipAddress, port)
		{};

	protected:

		/***************************************/
		/* Handler for client connections.	   */
		/***************************************/
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
		int	onMessageReceived(int clientSocket, const char *msg, int length)
		{
			return 0;
			//broadcastToClients(clientSocket, msg, length);
		};

	private:

};

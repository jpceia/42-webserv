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

			/*****************************************************/
            /* Parse out the document requested from the client. */
			/*****************************************************/
			std::istringstream iss(msg);
			std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)),
											 std::istream_iterator<std::string>());

			/******************************************************/
			/* Content has the information to send to the client. */
			/* htmlFile is the htmlFile to topen.                 */
			/* errorCode can be 200, 404, etc...                  */
			/******************************************************/
			std::string content = "";
			std::string htmlFile = "";
			int errorCode;

			/*********************************/
			/* If you received a GET.        */
			/*								 */
			/*   "GET /index.html HTTP/1.1"  */
			/*	  [0]     [1]        [2]     */
			/*********************************/
			if (parsed.size() >= 3 && parsed[0] == "GET")
			{
				/****************************/
				/* GET /index.html HTTP/1.1 */
				/* [0]     [1]       [2]    */
				/****************************/
				htmlFile = parsed[1];

				/******************************************/
				/* Calling localhost:8080 without a path. */
				/******************************************/
				if (htmlFile == "/")
				{
					htmlFile = "/index.html";
				}
			}

			/***********************************************/
            /* Open the document in the local file system. */
			/***********************************************/
			std::ifstream f("./wwwroot" + htmlFile);

			if (f.good())
			{
				std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
				content = str;
				errorCode = 200;
			}
			else
			{
				content = "<h1>404 Not Found</h1>";
				errorCode = 404;
			}

			f.close();

			/******************************************/
            /* Write the document back to the client. */
			/******************************************/
			std::ostringstream oss;
			oss << "HTTP/1.1 " << errorCode << " OK\r\n";
			oss << "Cache-Control: no-cache, private\r\n";
			oss << "Content-Type: text/html\r\n";
			oss << "Content-Length: " << content.size() << "\r\n";
			oss << "\r\n";
			oss << content;

            std::string output = oss.str();
            int size = output.size() + 1;

            int send_ret = sendToClient(clientSocket, output.c_str(), size);
            return (send_ret);
		};

	private:

};

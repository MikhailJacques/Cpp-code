#pragma once

#include <string>
#include <WS2tcpip.h>						// Header file for Winsock functions
#include "common.h"

#pragma comment(lib, "ws2_32.lib")			// Winsock library file

class UDP_Client
{
	public:

		UDP_Client(std::string server_ip, int server_port);
		~UDP_Client();
		bool Init(void);						// Initializes WinSock
		bool Send(std::string msg);				// Sends a message to the specified server
		void Cleanup(void);						// Cleans up after using the winsock service

	private:

		int	m_server_port;						// Server port
		std::string m_server_ip;				// Server IP address
		sockaddr_in m_server_info;				// Server metadata
		char m_server_msg_buff[MSG_BUFF_LEN];	// Buffer to store outgoing message to the server
		SOCKET m_client_socket;					// Client socket descriptor
		std::string m_client_status_msg;		// Client status message

		void Print_Client_Status_Msg(void);		// Prints client status message
};
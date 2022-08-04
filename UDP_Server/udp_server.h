#pragma once

#include <string>
#include <WS2tcpip.h>						// Header file for Winsock functions
#include "common.h"

#pragma comment(lib, "ws2_32.lib")			// Winsock library file

// Forward declaration of the class
class UDP_Server;

// Callback to a message received from a client
typedef void(*Rx_Msg_Handler)(UDP_Server* server_ptr, std::string client_msg);

class UDP_Server
{
public:

	UDP_Server(int server_port, Rx_Msg_Handler handler);
	~UDP_Server();
	bool Init(void);							// Initializes WinSock
	void Run(void);								// Executes the main processing loop
	bool Send(std::string msg);					// Sends a message to the client
	void Cleanup(void);							// Cleans up after using the winsock service
	void Print_Client_Msg(std::string msg);		// Prints a message received from a client to the output console

private:

	int	m_server_port;							// Server port
	SOCKET m_server_socket;						// Server socket descriptor
	std::string m_server_status_msg;			// Server status message
	int	m_client_port;							// Client port
	char m_client_ip[ADDRESS_BUFF_LEN];			// Client IP address
	sockaddr_in m_client_info;					// Populated by recvfrom with client address information (port / IP address)
	SOCKET m_client_socket;						// Client socket descriptor
	char m_client_msg_buff[MSG_BUFF_LEN];		// Client message buffer

	Rx_Msg_Handler rx_msg_handler;				// Message received event handler
	void Print_Server_Status_Msg(void);			// Prints server status message
};
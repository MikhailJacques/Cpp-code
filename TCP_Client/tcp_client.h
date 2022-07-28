// AUTHOR:		Mikhail Jacques
// PROJECT:		TCP
// DOCUMENT:	None
// DESCRIPTION: This file declares TCP client that sends messages

#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <WS2tcpip.h>				// Header file for Winsock functions

using namespace std;

#define _WINSOCKAPI_				// Stops windows.h including winsock
#include "windows.h"

#pragma comment(lib, "ws2_32.lib")	// Winsock library file

// Algorithm:
// Initialize winsock
// Create a socket
// Fill in a hint structure with server information
// Connect to a server
// Do-while loop to send and receive data
// Gracefully close down everything

static const unsigned short MSG_BUFF_LEN = (1024);

class TCP_Client
{
public:

	TCP_Client();
	TCP_Client(string server_ip, int server_port);
	~TCP_Client();
	void Set_Server_IP(string server_ip);				// Sets server IP address
	void Set_Server_Port(unsigned short server_port);	// Sets server port number
	bool Init(void);					// Initializes socket and connects to client; returns true on success; false otherwise
	bool Send(string msg);								// Sends a message to the specified server
	bool Receive(string& rx_msg);						// Receives a message from the specified server
	void Cleanup(void);									// Cleans up after using the winsock service

private:

	string m_server_ip;						// Server IP address
	unsigned short m_server_port;			// Server port
	sockaddr_in m_server_info;				// Server metadata
	char m_client_msg_buff[MSG_BUFF_LEN];	// Buffer to store outgoing message to the server
	char m_server_msg_buff[MSG_BUFF_LEN];	// Buffer to store incoming message from the server

	SOCKET m_client_socket;					// Client socket descriptor
	string m_sts_msg;						// Client status message

	void Print_Msg(string msg);				// Prints a status message
};

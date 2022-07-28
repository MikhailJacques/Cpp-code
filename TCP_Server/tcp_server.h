// AUTHOR:		Mikhail Jacques
// PROJECT:		TCP
// DOCUMENT:	None
// DESCRIPTION: This file declares TCP server that accepts messages

#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <WS2tcpip.h>						// Header file for Winsock functions
#include "tcp_server.h"
#pragma comment(lib, "ws2_32.lib")			// Winsock library file

using namespace std;

#define MAX_BUFFER_SIZE (1024)	// 8192

// Forward declaration of the class
class TCP_Server;

// Callback to data received
typedef void(*RxMsgHandler)(TCP_Server* tcp_server_ptr, SOCKET client_socket, string msg);

// Algorithm:
// Initialize winsock
// Create a socket
// Bind the IP address and port to a socket
// Tell Winsock the socket is for listening
// Wait for a connection
// Close listening socket
// While loop: receive message from and send message to client
// Close the socket

class TCP_Server
{
public:

	TCP_Server(string server_ip, int server_port, RxMsgHandler rx_msg_handler);
	~TCP_Server();
	bool Init(void);						// Initializes winsockand creates a listening socket
	bool Connect(void);						// Waits for a connection on a listening socket and returns a client socket
	void GetClientInfo(void);				// Retrieves client's info
	int Receive(void);						// Receives a message on the connected client socket
	bool Send(SOCKET client_socket, string msg); // Sends a message to the connected client socket
	void Cleanup(void);						// Cleans up after using the service

private:

	SOCKET CreateListeningSocket();			// Creates a listening socket
	void Print_Msg(string msg);				// Prints a status message

	string	m_server_ip;					// Server IP address
	int	m_server_port;						// Server listening port
	SOCKET m_listening_socket;				// Server listening socket
	SOCKET m_client_socket;					// Client socket
	sockaddr_in m_client_info;				// Client information
	RxMsgHandler m_rx_msg_handler;			// Received message event handler	
};

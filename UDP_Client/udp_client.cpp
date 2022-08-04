#include <sstream>
#include <iostream>
#include "UDP_Client.h"

UDP_Client::UDP_Client(std::string server_ip, int server_port) :
	m_server_ip(server_ip),
	m_server_port(server_port),
	m_client_socket(INVALID_SOCKET),
	m_client_status_msg(""),
	m_server_msg_buff("\0")
{ }

UDP_Client::~UDP_Client()
{
	Cleanup();
}

// This function initializes WinSock
// Returns true on success; false otherwise
bool UDP_Client::Init()
{
	WSAData data;
	WORD version = MAKEWORD(2, 2);
	bool ret_val = false;		// true = success; false = failure

	// Start WinSock
	int wsa_ret_val = WSAStartup(version, &data);

	if (wsa_ret_val != 0)
	{
		m_client_status_msg.assign("ERROR: WSAStartup() failed");
		Print_Client_Status_Msg();
	}
	else
	{
		// Create client sending socket: params: IPv4 address famly, datagram socket, UDP
		m_client_socket = socket(AF_INET, SOCK_DGRAM, 0);

		// Check to see whether socket creation succeeded
		if (m_client_socket == INVALID_SOCKET)
		{
			m_client_status_msg.assign("ERROR: socket() failed %ld", WSAGetLastError());
			Print_Client_Status_Msg();
		}
		else
		{
			// Initialize m_server_info structure with server IP address and port number
			memset((char*)&m_server_info, 0, sizeof(m_server_info));
			m_server_info.sin_family = AF_INET;							// Address format is IPv4
			m_server_info.sin_port = htons((u_short)m_server_port);
			// Convert internet network address from text presentation into numeric binary form
			inet_pton(AF_INET, m_server_ip.c_str(), &m_server_info.sin_addr);

			ret_val = true;
		}
	}

	return ret_val;
}

// The main processing loop
bool UDP_Client::Send(std::string msg)
{
	bool ret_val = true;		// true = success; false = failure

	if (m_client_socket == INVALID_SOCKET)
	{
		ret_val = false;

		return ret_val;
	}

	// Fill in the buffer with the outgoing message
	memcpy(m_server_msg_buff, msg.c_str(), msg.size() + 1);

	int num_of_tx_bytes = sendto(m_client_socket, m_server_msg_buff, strlen(m_server_msg_buff), 0, (sockaddr*)&m_server_info, sizeof(m_server_info));

	if (num_of_tx_bytes == SOCKET_ERROR)
	{
		m_client_status_msg.assign("ERROR: send() failed %ld", WSAGetLastError());
		Print_Client_Status_Msg();
		ret_val = false;
	}

	return ret_val;
}

void UDP_Client::Cleanup(void)
{
	int ret_val = closesocket(m_client_socket);

	if (ret_val == SOCKET_ERROR)
	{
		m_client_status_msg.assign("ERROR: closesocket() failed %ld", WSAGetLastError());
		Print_Client_Status_Msg();
	}

	if (ret_val == SOCKET_ERROR)
	{
		m_client_status_msg.assign("ERROR: WSACleanup() failed %ld", WSAGetLastError());
		Print_Client_Status_Msg();
	}
}

// This function prints out a status message
void UDP_Client::Print_Client_Status_Msg(void)
{
	std::cout << m_client_status_msg << "\n";
}
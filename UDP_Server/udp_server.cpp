#include <sstream>
#include <iostream>
#include "udp_server.h"

UDP_Server::UDP_Server(int server_port, Rx_Msg_Handler handler) :
	m_server_port(server_port),
	m_server_socket(INVALID_SOCKET),
	m_server_status_msg(""),
	m_client_ip("\0"),
	m_client_port(0),
	m_client_msg_buff("\0"),
	rx_msg_handler(handler)
{ }

UDP_Server::~UDP_Server()
{
	Cleanup();
}

// This function initializes WinSock
// Returns true on success; false otherwise
bool UDP_Server::Init()
{
	WSAData data;
	WORD version = MAKEWORD(2, 2);
	bool ret_val = false;		// true = success; false = failure

	// Start WinSock
	int wsa_ret_val = WSAStartup(version, &data);

	if (wsa_ret_val != 0)
	{
		m_server_status_msg.assign("ERROR: WSAStartup() failed");
		Print_Server_Status_Msg();
	}
	else
	{
		// Create server receiving socket: IPv4 address family, datagram socket, UDP
		m_server_socket = socket(AF_INET, SOCK_DGRAM, 0);

		// Create client sending socket: params: IPv4 address family, datagram socket, UDP
		m_client_socket = socket(AF_INET, SOCK_DGRAM, 0);

		// Check to see whether socket creation succeeded
		if ((m_server_socket == INVALID_SOCKET) || (m_client_socket == INVALID_SOCKET))
		{
			m_server_status_msg.assign("ERROR: socket() failed %ld", WSAGetLastError());
			Print_Server_Status_Msg();
		}
		else
		{
			// Create a server hint structure for the server
			// Create structure to bind IP address and port number to the listening socket
			sockaddr_in hint;
			hint.sin_family = AF_INET;				// Address format is IPv4
			hint.sin_addr.S_un.S_addr = ADDR_ANY;	// Use any IP address available on the machine
			hint.sin_port = htons(m_server_port);	// Convert from little to big endian

			// Convert internet network address from its standard text presentation form into its numeric binary form
			// inet_pton(AF_INET, m_ip_address.c_str(), &hint.sin_addr);

			// Bind the server socket to the IP address and port
			int bind_ret_val = bind(m_server_socket, (sockaddr*)&hint, sizeof(hint));

			if (bind_ret_val == SOCKET_ERROR)
			{
				m_server_status_msg.assign("ERROR: bind() failed %ld", WSAGetLastError());
				Print_Server_Status_Msg();
				m_server_socket = INVALID_SOCKET;
			}
			else
			{
				// Set socket to nonblocking mode
				unsigned long int nonblocking_mode = 1;
				int ioctlsocket_ret_val = ioctlsocket(m_server_socket, FIONBIO, &nonblocking_mode);

				if (ioctlsocket_ret_val == SOCKET_ERROR)
				{
					m_server_status_msg.assign("ERROR: ioctlsocket() failed %ld", WSAGetLastError());
					Print_Server_Status_Msg();
				}
				else
				{
					m_server_status_msg.assign("SERVER: Welcome");
					Print_Server_Status_Msg();
					ret_val = true;
				}
			}
		}
	}

	return ret_val;
}

// The main processing loop
void UDP_Server::Run(void)
{
	if (m_server_socket == INVALID_SOCKET)
	{
		m_server_status_msg.assign("ERROR: socket() failed %ld", WSAGetLastError());
		Print_Server_Status_Msg();
		return;
	}

	int client_len = sizeof(m_client_info);				// The size of the client information
	ZeroMemory(&m_client_info, client_len);				// Clear the client structure

	// Enter data receiving loop
	while (true)
	{
		// Declare select parameters
		FD_SET set;
		timeval time_val;

		// Initialize select parameters
		FD_ZERO(&set);
		FD_SET(m_server_socket, &set);					// Add socket we will wait to read from

		// Set timeouts to zero in order for the select() to return instantaneously
		time_val.tv_sec = 0;
		time_val.tv_usec = 0;

		int select_ret_val = select(0 /* ignored */, &set, NULL, NULL, &time_val);

		// Check to see if there was an error during select
		if (select_ret_val == SOCKET_ERROR)
		{
			m_server_status_msg.assign("ERROR: select() failed %ld", WSAGetLastError());
			Print_Server_Status_Msg();

			continue;
		}

		// Check to see if there are any sockets ready
		if (select_ret_val == 0)
		{
			// There are no ready sockets, sleep for a while and check again
			Sleep(50);
			continue;
		}

		// Clear the receive buffer
		ZeroMemory(m_client_msg_buff, MSG_BUFF_LEN);

		// Receive a message from a client and store the associated client's metadata in the client_info structure
		int num_of_rx_bytes = recvfrom(m_server_socket, m_client_msg_buff, MSG_BUFF_LEN, 0, (sockaddr*)&m_client_info, &client_len);

		if (num_of_rx_bytes == SOCKET_ERROR)
		{
			m_server_status_msg.assign("ERROR: recvfrom() failed %ld", WSAGetLastError());
			Print_Server_Status_Msg();

			continue;
		}

		if (num_of_rx_bytes > 0)
		{
			// Create enough space to convert the address byte array to string of characters
			ZeroMemory(m_client_ip, ADDRESS_BUFF_LEN);

			// Retrieve client's IP address (convert network address from byte representation into a string in the Internet standard format)
			inet_ntop(AF_INET, &m_client_info.sin_addr, m_client_ip, ADDRESS_BUFF_LEN);

			// Retrieve client's port number (convert from network byte order to host byte order)
			m_client_port = ntohs(m_client_info.sin_port);

			if (rx_msg_handler != NULL)
			{
				rx_msg_handler(this, std::string(m_client_msg_buff, 0, num_of_rx_bytes));

				typedef void(*Rx_Msg_Handler)(UDP_Server* server_ptr, std::string client_msg);

			}
		}
	}
}

// This function sends a message to the client
bool UDP_Server::Send(std::string msg)
{
	bool ret_val = true;		// true = success; false = failure

	if (m_client_socket == INVALID_SOCKET)
	{
		ret_val = false;
		return ret_val;
	}

	int num_of_tx_bytes = sendto(m_client_socket, msg.c_str(), msg.size() + 1, 0, (sockaddr*)&m_client_info, sizeof(m_client_info));

	if (num_of_tx_bytes == SOCKET_ERROR)
	{
		m_server_status_msg.assign("ERROR: send() failed %ld", WSAGetLastError());
		Print_Server_Status_Msg();
		ret_val = false;
	}

	return ret_val;
}

// This function cleans up used resources
void UDP_Server::Cleanup(void)
{
	int ret_val = closesocket(m_server_socket);

	if (ret_val == SOCKET_ERROR)
	{
		m_server_status_msg.assign("ERROR: closesocket() failed %ld", WSAGetLastError());
		Print_Server_Status_Msg();
	}

	ret_val = WSACleanup();

	if (ret_val == SOCKET_ERROR)
	{
		m_server_status_msg.assign("ERROR: WSACleanup() failed %ld", WSAGetLastError());
		Print_Server_Status_Msg();
	}
}

// This function prints out server status message
void UDP_Server::Print_Server_Status_Msg(void)
{
	std::cout << m_server_status_msg << "\n";
}

// This function prints a message received from a client
void UDP_Server::Print_Client_Msg(std::string msg)
{
	// Display client info and its message
	std::cout << "CLIENT: " << m_client_ip << ":" << m_client_port << " : " << m_client_msg_buff << "\n";
}
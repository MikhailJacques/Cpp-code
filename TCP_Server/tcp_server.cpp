// AUTHOR:		Mikhail Jacques
// PROJECT:		TCP
// DOCUMENT:	None
// DESCRIPTION: This file defines TCP server that accepts messages

#include "TCP_Server.h"

TCP_Server::TCP_Server(string server_ip, int server_port, RxMsgHandler rx_msg_handler) :
	m_server_ip(server_ip),
	m_server_port(server_port),
	m_rx_msg_handler(rx_msg_handler),
	m_listening_socket(INVALID_SOCKET),
	m_client_socket(INVALID_SOCKET)
{
	m_client_info = {};	// memset(&m_client_info, 0, sizeof(m_client_info));

	if (m_rx_msg_handler == NULL)
	{
		Print_Msg("SERVER ERROR: Constructor(): m_rx_msg_handler is NULL");
	}
}

TCP_Server::~TCP_Server()
{
	Cleanup();
}

// This function initializes winsock ans creates a listening socket
bool TCP_Server::Init(void)
{
	// The WSADATA structure contains information about the Windows sockets implementation
	WSADATA wsa_data;
	WORD ver = MAKEWORD(2, 2);
	
	int wsa_init_sts = WSAStartup(ver, &wsa_data);

	if (wsa_init_sts != 0)
	{
		stringstream ss;
		ss << "SERVER ERROR: Init() failed: " << wsa_init_sts;
		Print_Msg(ss.str());
		return false;
	}
	else
	{
		if (CreateListeningSocket() == false)
		{
			Print_Msg("SERVER ERROR: CreateListeningSocket() failed!");
			return false;
		}
	}

	Print_Msg("SERVER: TCP server is up");

	return true;
}

// This function creates a listening socket
SOCKET TCP_Server::CreateListeningSocket(void)
{
	bool ret_val = true;

	m_listening_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (m_listening_socket != INVALID_SOCKET)
	{
		// Create structure to bind the IP address and port number to the listening socket
		// In order for a server to communicate on a network, it must bind the socket to a network address.
		// Need to assemble the required data for connection in sockaddr structure.
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(m_server_port);	// host to network short
		inet_pton(AF_INET, m_server_ip.c_str(), &hint.sin_addr);

		// Bind the IP address and port to the listening socket
		int bind_sts = bind(m_listening_socket, (sockaddr*)&hint, sizeof(hint));

		if (bind_sts != SOCKET_ERROR)
		{
			// Tell Winsock the socket is for listening
			int listen_sts = listen(m_listening_socket, SOMAXCONN);

			if (listen_sts == SOCKET_ERROR)
			{
				ret_val = false;
			}
		}
		else
		{
			ret_val = false;
		}
	}

	return ret_val;
}

// This function waits for a connection on a listening socket and 
// upon establishing the connection creates a connected client socket
bool TCP_Server::Connect(void)
{
	bool ret_val = true;
	int client_size = sizeof(m_client_info);

	// The accept function permits an incoming connection attempt on a socket.
	m_client_socket = accept(m_listening_socket, (sockaddr*)&m_client_info, &client_size);

	if (m_client_socket == INVALID_SOCKET)
	{
		stringstream ss;
		ss << "SERVER ERROR: accept() failed: " << WSAGetLastError();
		Print_Msg(ss.str());

		ret_val = false;
	}
	else
	{
		// Once a client is connected, close the listening socket so that no other clients can connect on it
		closesocket(m_listening_socket);
	}

	return ret_val;
}

void TCP_Server::GetClientInfo(void)
{
	// Code from bare-bones echo server 
	char host[NI_MAXHOST];				// Client's remote name
	char client_port[NI_MAXHOST];		// Client's port

	ZeroMemory(host, NI_MAXHOST);		// memset(host, 0, NI_MAXHOST);
	ZeroMemory(client_port, NI_MAXHOST);// memset(service, 0, NI_MAXHOST);

	// Get client's host name (DNS lookup)
	// The getnameinfo function is used to translate the contents of a socket address structure to a node name and/or a service name. 
	if (getnameinfo((sockaddr*)&m_client_info, sizeof(m_client_info), host, NI_MAXHOST, client_port, NI_MAXHOST, 0) == 0)
	{
		cout << "SERVER: " << host << " connected on port " << client_port << endl;
	}
	else
	{
		inet_ntop(AF_INET, &m_client_info.sin_addr, host, NI_MAXHOST);
		cout << "SERVER: " << host << " connected on port" << ntohs(m_client_info.sin_port) << endl;
	}
}

// This function receives data from a client on the specified client socket
int TCP_Server::Receive(void)
{
	string rx_msg;
	int num_of_rx_bytes = 0;
	char rx_buff[MAX_BUFFER_SIZE];

	do
	{
		ZeroMemory(rx_buff, MAX_BUFFER_SIZE);

		// Wait to receive data from the client
		num_of_rx_bytes = recv(m_client_socket, rx_buff, MAX_BUFFER_SIZE, 0);

		if (num_of_rx_bytes == SOCKET_ERROR)
		{
			Print_Msg("SERVER ERROR: recv() failed!");
			break;
		}

		if (num_of_rx_bytes == 0)
		{
			Print_Msg("SERVER: Client disconnected");
			break;
		}

		if (num_of_rx_bytes > 0)
		{
			if (m_rx_msg_handler != NULL)
			{
				rx_msg = string(rx_buff, 0, num_of_rx_bytes);

				m_rx_msg_handler(this, m_client_socket, rx_msg);
			}
		}

	} while (num_of_rx_bytes > 0);
	// while ((num_of_rx_bytes > 0) && (rx_msg.compare("done") != 0));

	return num_of_rx_bytes;
}

// This function sends data on a connected client socket
bool TCP_Server::Send(SOCKET client_socket, string msg)
{
	bool ret_val = true;

	int num_of_tx_bytes = send(client_socket, msg.c_str(), (int)msg.size(), 0);

	if (num_of_tx_bytes == SOCKET_ERROR)
	{
		stringstream ss;
		ss << "SERVER ERROR: send() failed: " << WSAGetLastError();
		Print_Msg(ss.str());
		ret_val = false;
	}

	return ret_val;
}

// This function shuts the server down
void TCP_Server::Cleanup(void)
{
	Print_Msg("SERVER: Shutting down");
	closesocket(m_client_socket);
	WSACleanup();
}

// This function prints out a status message
void TCP_Server::Print_Msg(string msg)
{
	cout << msg << endl;
}
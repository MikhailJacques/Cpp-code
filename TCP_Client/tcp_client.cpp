// AUTHOR:		Mikhail Jacques
// PROJECT:		TCP
// DOCUMENT:	None
// DESCRIPTION: This file defines TCP client that sends messages

#include "tcp_client.h"

TCP_Client::TCP_Client() :
	m_server_ip(""),
	m_server_port(0),
	m_client_socket(INVALID_SOCKET),
	m_sts_msg(""),
	m_client_msg_buff("\0"),
	m_server_msg_buff("\0")
{ }

TCP_Client::TCP_Client(string server_ip, int server_port) :
	m_server_ip(server_ip),
	m_server_port(server_port),
	m_client_socket(INVALID_SOCKET),
	m_sts_msg(""),
	m_client_msg_buff("\0"),
	m_server_msg_buff("\0")
{ }

TCP_Client::~TCP_Client()
{
	Cleanup();
}

void TCP_Client::Set_Server_IP(string server_ip)
{
	m_server_ip.assign(server_ip);
}

void TCP_Client::Set_Server_Port(unsigned short server_port)
{
	m_server_port = server_port;
}

bool TCP_Client::Init(void)
{
	WSAData data;
	WORD version = MAKEWORD(2, 2);
	stringstream ss;
	bool init_flag = false;		// true: client has been initialized; false otherwise

	// Start WinSock
	int wsa_ret_val = WSAStartup(version, &data);

	if (wsa_ret_val != 0)
	{
		ss << "CLIENT ERROR: WSAStartup() failed: " << wsa_ret_val;
		Print_Msg(ss.str());
	}
	else
	{
		// Create client sending socket
		m_client_socket = socket(AF_INET, SOCK_STREAM, 0);

		// Check to see whether socket creation succeeded
		if (m_client_socket == INVALID_SOCKET)
		{
			ss << "CLIENT ERROR: socket() failed: " << WSAGetLastError();
			Print_Msg(ss.str());
		}
		else
		{
			// Initialize m_server_info structure with server IP address and port number information
			memset((char*)&m_server_info, 0, sizeof(m_server_info));
			m_server_info.sin_family = AF_INET;							// Address format is IPv4
			m_server_info.sin_port = htons((u_short)m_server_port);

			// Convert IPv4 and IPv6 internet network address from text presentation into numeric binary form	
			if (inet_pton(AF_INET, m_server_ip.c_str(), &m_server_info.sin_addr) <= 0)
			{
				ss << "CLIENT ERROR: Invalid network address: " << WSAGetLastError();
				Print_Msg(ss.str());
			}
			else
			{
				// Connect to a server
				int connect_sts = connect(m_client_socket, (struct sockaddr*)&m_server_info, sizeof(m_server_info));

				if (connect_sts == SOCKET_ERROR)
				{
					ss << "CLIENT ERROR: Connection to server failed: " << WSAGetLastError();
					Print_Msg(ss.str());
				}
				else
				{
					init_flag = true;
				}
			}
		}
	}

	return init_flag;
}

// The main processing loop
bool TCP_Client::Send(string msg)
{
	bool ret_val = true;	// true = success; false = failure

	if (m_client_socket == INVALID_SOCKET)
	{
		return false;
	}

	// Fill in the buffer with the outgoing message
	memcpy(m_client_msg_buff, msg.c_str(), msg.size() + 1);

	// ssize_t write(int fd, const void *buf, size_t count);
	// ssize_t send(int sockfd, const void *buf, size_t len, int flags);
	// sendto(m_client_socket, m_client_msg_buff, strlen(m_client_msg_buff), 0, (sockaddr*)&m_server_info, sizeof(m_server_info));

	int num_of_tx_bytes = send(m_client_socket, m_client_msg_buff, (int)strlen(m_client_msg_buff), 0);

	if (num_of_tx_bytes == SOCKET_ERROR)
	{
		stringstream ss;
		ss << "CLIENT ERROR: send() failed: " << WSAGetLastError();
		Print_Msg(ss.str());
		ret_val = false;
	}
	else
	{
		Print_Msg("CLIENT: Message has been sent");
	}

	return ret_val;
}

bool TCP_Client::Receive(string& rx_msg)
{
	stringstream ss;
	bool ret_val = true;

	// int bytes_received = read(m_client_socket, m_server_msg_buff, MSG_BUFF_LEN);
	int num_of_rx_bytes = recv(m_client_socket, m_server_msg_buff, MSG_BUFF_LEN, NULL);

	if (num_of_rx_bytes < 0)
	{
		Print_Msg("CLIENT ERROR: recv() failed!");
		ret_val = false;
	}

	if (num_of_rx_bytes > 0)
	{
		rx_msg.assign(m_server_msg_buff, num_of_rx_bytes);
	}

	return ret_val;
}

void TCP_Client::Cleanup(void)
{
	int ret_val = closesocket(m_client_socket);

	if (ret_val == SOCKET_ERROR)
	{
		stringstream ss;
		ss << "CLIENT ERROR: closesocket() failed: " << WSAGetLastError();
		Print_Msg(ss.str());
	}
}

// This function prints out a status message
void TCP_Client::Print_Msg(string msg)
{
	cout << msg << endl;
}
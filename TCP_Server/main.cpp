// AUTHOR:		Mikhail Jacques
// PROJECT:		TCP
// DOCUMENT:	
// DESCRIPTION: This file defines entry point of the application

#include <string>
#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include "tcp_server.h"

using namespace std;

void Rx_Msg_Handler(TCP_Server* server, SOCKET client_socket, string msg);

int main()
{
	const string server_ip = "127.0.0.1";		// Server IP address
	const unsigned short server_port = 5678;	// Server listening port		

	TCP_Server server(server_ip, server_port, Rx_Msg_Handler);

	if (server.Init())
	{
		// Connect with a client on a listening socket
		if (server.Connect() == true)
		{
			server.GetClientInfo();

			server.Receive();
		}
	}

	return 0;
}

void Rx_Msg_Handler(TCP_Server* server, SOCKET client_socket, string msg)
{
	cout << "SERVER: Message received: " << msg << endl;

	string tx_msg;

	// Read user input
	cout << "SERVER: Message to send: ";
	getline(cin, tx_msg);

	if (server->Send(client_socket, tx_msg))
	{
		cout << "SERVER: Message has been sent\n";
	}

	//// Read user input
	//do
	//{
	//	cout << "SERVER: Message to send: ";
	//	getline(cin, tx_msg);

	//	if (tx_msg.compare("done") == 0)
	//		break;

	//	server->Send(client_socket, tx_msg);

	//} while (true);
}

// Auxiliary function
//string ConvertToString(char* a, int size)
//{
//	string s = "";
//
//	for (int i = 0; i < size; i++)
//		s = s + a[i];
//
//	return s;
//}
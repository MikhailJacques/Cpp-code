// AUTHOR:		Mikhail Jacques
// PROJECT:		TCP
// DOCUMENT:	
// DESCRIPTION: This file defines entry point of the application

#include <iostream>
#include <sstream>

#include "tcp_client.h"

using namespace std;

int main()
{
	string msg;
	const string server_ip = "127.0.0.1";		// Server IP address
	const unsigned short server_port = 5678;	// Server listening port		
	
	TCP_Client client(server_ip, server_port);

	if (client.Init())
	{
		cout << "CLIENT: TCP client is up" << endl;
	}

	// Read user input
	do
	{
		cout << "CLIENT: Message to send: ";
		getline(cin, msg);

		if (msg.compare("done") == 0)
		{
			cout << "CLIENT: Received program termination command from UI console\n" 
				<< "CLIENT: Shutting down";

			break;
		}

		if (client.Send(msg) == true)
		{
			string msg_rx;

			if (client.Receive(msg_rx) == true)
			{
				cout << "CLIENT: Message received: " << msg_rx << endl;
			}
		}
	} while (true);

	return 0;
}
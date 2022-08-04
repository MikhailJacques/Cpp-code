#include <iostream>
#include <string>

#include "UDP_Server.h"

void UDP_Server_Msg_Rx(UDP_Server* server_ptr, std::string client_msg);

int main()
{
	UDP_Server udp_server(SERVER_PORT, UDP_Server_Msg_Rx);

	if (udp_server.Init())
	{
		udp_server.Run();
	}
}

void UDP_Server_Msg_Rx(UDP_Server* server_ptr, std::string client_msg)
{
	server_ptr->Print_Client_Msg(client_msg);
}
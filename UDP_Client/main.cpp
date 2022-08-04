#include <iostream>
#include <string>

#include "udp_client.h"

int main()
{
	UDP_Client udp_client(LOCALHOST, SERVER_PORT);

	if (udp_client.Init())
	{
		udp_client.Send("Hello server!");
	}
}
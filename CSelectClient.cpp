// CSelectClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Client.h"

int main()
{
	std::string asdf = "asdf";

	CPackets packet;
	packet << asdf;

	CClient Client;
	Client.Init("127.0.0.1", 9000);
	Client.Connect();

	while (1)
	{
		Client.selectEvent();
		Sleep(5);
	}
    return 0;
}


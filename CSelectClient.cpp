// CSelectClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Client.h"
#include "CriticalSections.h"
int main()
{

	CClient Client;
	Client.Init("127.0.0.1", 9000);
	Client.Connect();
	CPacket packet;
	while (1)
	{
		CriticalSections::getInstance()->enter();
		Client.CopyMessageQue();
		for (auto packet : Client.getQue().messageQue)
		{
			Client.getQue().packetParsing(packet);
		}
		Client.getQue().messageQue.clear();
		CriticalSections::getInstance()->leave();
	}

    return 0;
}


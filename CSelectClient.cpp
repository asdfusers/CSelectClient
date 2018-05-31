// CSelectClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Client.h"
#include "CriticalSections.h"
#include "ThreadManager.h"
int main()
{

	CClient Client;
	Client.Init("127.0.0.1", 9000);
	Client.Connect();
	Client._SelectThread.begin();
	

	while (1)
	{
		if (!Client._SelectThread.getQue().recvQue.empty())
		{		
			Client.CopyMessageQue();
			Client.getQue().packetParsing(Client.getQue().recvQue.front());
			Client.getQue().recvQue.pop();
		}
		if (!Client.getQue().sendQue.empty())
		{
			Client.CopySendMessageQue();
			Client.getSendQue().SendMessageW();
		}
		Sleep(1);
	}
	CThreadManager::getInstance()->join();

	
    return 0;
}


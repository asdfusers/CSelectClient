// CSelectClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Client.h"
#include "CriticalSections.h"
#include "CriticalSectionLock.h"



int main()
{

	CClient Client;
	Client.Init("127.0.0.1", 9000);
	Client.Connect();
	Client._SelectThread.begin();
	

	while (1)
	{
		Client.Update();
		Client.Print();
		if (Client.bGame)
			Client.Render();
		Sleep(1);
	}

    return 0;
}

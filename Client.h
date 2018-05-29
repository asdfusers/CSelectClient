#pragma once
#include "stdafx.h"
#include "Packet.h"
#include "SelectThread.h"


class CClient
{
public:
	CClient();
	~CClient();

	bool Init(std::string IP, int PORT);
	bool Connect();
	void CopyMessageQue();

	CMessageQueue getQue() { return _MessageQue; }

	CSelectThread _SelectThread;
private:
	CMessageQueue _MessageQue;
	SOCKET mSocket;
	SOCKADDR_IN addr;
	int addrLen;
	int retVal;

	std::list<char*> messageQue;

};


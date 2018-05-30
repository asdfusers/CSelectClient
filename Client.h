#pragma once
#include "stdafx.h"
#include "Packet.h"
#include "SelectThread.h"

#include "RecvQue.h"
#include "SendQue.h"


class CClient
{
public:
	CClient();
	~CClient();

	bool Init(std::string IP, int PORT);
	bool Connect();
	void CopyMessageQue();
	void CopySendMessageQue();
	CSelectThread _SelectThread;
	CRecvQue& getQue() { return recvQue; }
	CSendQue& getSendQue() { return sendQue; }
	CriticalSections cs;
private:
	
	CRecvQue recvQue;
	CSendQue sendQue;
	
	SOCKET mSocket;
	SOCKADDR_IN addr;
	int addrLen;
	int retVal;

	std::list<char*> messageQue;

};


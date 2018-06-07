#pragma once
#include "stdafx.h"
#include "Packet.h"
#include "SelectThread.h"

#include "CriticalSectionLock.h"

class CClient
{
public:
	CClient();
	~CClient();

	bool Init(std::string IP, int PORT);
	bool Connect();
	void CopyMessageQue();
	
	CSelectThread _SelectThread;
	CS::CriticalSection cs;

	bool sendMessage(CPacket packet);

	void Update();


	void packetParsing(CPacket packet);
	void onPConnectionSuccessAck(CPacket & packet);
	void onPSelectLobbyOption(CPacket & packet);
	void onPSelectLobby(CPacket & packet);
	void onPTestPacket3Ack(CPacket & packet);

private:
	SOCKET mSocket;
	SOCKADDR_IN addr;
	int addrLen;
	int retVal;
	CMessageQue recvQue;
	CMessageQue sendQue;

};


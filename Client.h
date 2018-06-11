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
	void CopyMessageSendQue();
	CSelectThread _SelectThread;
	CS::CriticalSection cs;

	bool sendMessage(CPacket packet);

	void Update();


	/*void packetParsing(CPacket packet);
	void onPConnectionSuccessAck(CPacket & packet);
	void onPSelectLobbyOption(CPacket & packet);
	void onPSelectLobby(CPacket & packet);
	void onPEnterRoom(CPacket & packet);
	void onPBroadCastEnterRoom(CPacket & packet);
	void onPReadyAck(CPacket & packet);
	void onPReadyResultAck(CPacket & packet);*/

	unsigned short packetHeader;
	void packetSend(unsigned short _packetHeader);

private:
	SOCKET mSocket;
	SOCKADDR_IN addr;
	int addrLen;
	int retVal;
	CMessageQue recvQue;
	CMessageQue sendQue;

	
	wchar_t cInput;
};


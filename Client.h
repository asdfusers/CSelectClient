#pragma once
#include "stdafx.h"
#include "Packet.h"
#include "SelectThread.h"
#include "KeyInput.h"
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
	CS::CriticalSection cs1;
	bool sendMessage(CPacket packet);

	void Update();
	void Print();
	void Render();

	void packetParsing(CPacket packet);
	void onPConnectionSuccessAck(CPacket & packet);
	void onPSelectLobbyOption(CPacket & packet);
	void onPSelectLobby(CPacket & packet);
	void onPEnterRoom(CPacket & packet);
	void onPBroadCastEnterRoom(CPacket & packet);
	void onPBroadCastReadyRoom(CPacket & packet);
	void onPReadyAck(CPacket & packet);
	void onPReadyResultAck(CPacket & packet);
	void onPGameStartAck(CPacket & packet);
	void onPGameStart(CPacket & packet);
	void onPGameInput(CPacket & packet);
	void onPEnemyPos(CPacket & packet);
	void onPMatching(CPacket & packet);

	

	unsigned short packetHeader;
	void packetSend(unsigned short _packetHeader);
	
	bool bGame = false;
private:
	wchar_t Buffer[127];
	SOCKET mSocket;
	SOCKADDR_IN addr;
	int addrLen;
	int retVal;
	CMessageQue recvQue;
	CMessageQue sendQue;
	std::queue<wchar_t*> printQue;
	CKeyInput Input;
	wchar_t cInput;

};


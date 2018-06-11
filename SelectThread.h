#pragma once
#include "stdafx.h"
#include "Thread.h"
#include "Packet.h"
#include "MessageQue.h"
#include "CriticalSectionLock.h"
#include "CriticalSections.h"
class CSelectThread : public CThread
{
public:
	CSelectThread();
	~CSelectThread();

public:
	virtual void threadMain();
	bool onReceive();
	void onClose();
	void setSocket(SOCKET _Sock) { mSocket = _Sock; }

	CMessageQue recvQue;
	CMessageQue sendQue;
	
	void packetParsing(CPacket packet);
	void onPConnectionSuccessAck(CPacket & packet);
	void onPSelectLobbyOption(CPacket & packet);
	void onPSelectLobby(CPacket & packet);
	void onPEnterRoom(CPacket & packet);
	void onPBroadCastEnterRoom(CPacket & packet);
	void onPReadyAck(CPacket & packet);
	void onPReadyResultAck(CPacket & packet);
	void onPGameStartAck(CPacket & packet);
	void onPGameStart(CPacket & packet);
	void onPGameInput(CPacket & packet);
	
	CS::CriticalSection cs;

private:
	SOCKET mSocket;

	int receivePacketSize;
	char receiveBuffer[PACKETBUFFERSIZE];
	WSAEVENT recvEvent;
	
	int retVal;


};


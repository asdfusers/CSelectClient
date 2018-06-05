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

	CS::CriticalSection cs;

private:
	SOCKET mSocket;

	int receivePacketSize;
	char receiveBuffer[PACKETBUFFERSIZE];
	WSAEVENT recvEvent;
	
	int retVal;


};


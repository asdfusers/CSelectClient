#pragma once
#include "stdafx.h"
#include "MessageQueue.h"
#include "Thread.h"
#include "Packet.h"

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

	std::list<CPacket> getQue() { return messageQueue; }
private:
	SOCKET mSocket;
	std::list<CPacket> messageQueue;
	
	int receivePacketSize;
	char receiveBuffer[MAX_BUFFER_SIZE];
	WSAEVENT recvEvent;
	
	int retVal;


};


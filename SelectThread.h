#pragma once
#include "stdafx.h"
#include "MessageQueue.h"
#include "Thread.h"
#include "Packet.h"
#include "RecvQue.h"
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

	CRecvQue& getQue() { return recvQue; }

private:
	SOCKET mSocket;
	CRecvQue recvQue;

	int receivePacketSize;
	char receiveBuffer[MAX_BUFFER_SIZE];
	WSAEVENT recvEvent;
	
	int retVal;


};


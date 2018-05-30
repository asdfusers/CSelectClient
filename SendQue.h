#pragma once
#include "CriticalSections.h"
#include "Packet.h"

#include <queue>


class CSendQue
{
public:
	CSendQue();
	~CSendQue();

public:
	void SetSocket(SOCKET _sock) { mSocket = _sock; }
	bool SendMessage();

public:
	SOCKET mSocket;
	std::queue<CPacket> sendQue;
	CriticalSections cs;
};


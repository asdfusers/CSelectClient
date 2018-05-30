#pragma once
#include "CriticalSections.h"
#include "Packet.h"

#include <queue>

class CRecvQue
{
public:
	CRecvQue();
	~CRecvQue();

public:
	void	packetParsing(CPacket packet);
	void	onPConnectionSuccessAck(CPacket& packet);
	void	onPTestPacket1Ack(CPacket& packet);
	void	onPTestPacket2Ack(CPacket& packet);
	void	onPTestPacket3Ack(CPacket& packet);

public:
	void setSocket(SOCKET _socket) { mSocket = _socket; }

public:
	std::queue<CPacket> recvQue;
	std::queue<CPacket> sendQue;
	CriticalSections cs;

private:
	SOCKET mSocket;
};


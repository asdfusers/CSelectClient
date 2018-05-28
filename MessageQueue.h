#include "stdafx.h"
#include "Packet.h"

class CMessageQueue
{
public:
	void	packetParsing(CPacket packet);
	void	onPConnectionSuccessAck(CPacket& packet);
	void	onPTestPacket1Ack(CPacket& packet);
	void	onPTestPacket2Ack(CPacket& packet);
	void	onPTestPacket3Ack(CPacket& packet);
	bool	SendPacket();


public:
	void setSocket(SOCKET _socket) { mSocket = _socket; }

public:
	std::list<CPacket> messageQue; // recvQue
	std::list<CPacket> sendQue; // sendQue;
	SOCKET mSocket;
};
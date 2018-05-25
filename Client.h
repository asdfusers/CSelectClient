#pragma once
#include "stdafx.h"
#include "Packets.h"
class CClient
{
public:
	CClient();
	~CClient();

	bool Init(std::string IP, int PORT);
	bool Connect();
	bool SendPacket(CPackets& packet);
	bool onReceive();
	void onClose();
	void selectEvent();
	void packetParsing(CPackets& packet);

	void	onPTConnectionSuccessAck(CPackets& packet);
	void	onPTTestPacket1Ack(CPackets& packet);
	void	onPTTestPacket2Ack(CPackets& packet);
	void	onPTTestPacket3Ack(CPackets& packet);

private:
	SOCKET mSocket;
	SOCKADDR_IN addr;
	int addrLen;
	int retVal;

	char receiveBuffer[PACKETBUFFERSIZE];
	int receivePacketSize;

	WSAEVENT recvEvent;

};


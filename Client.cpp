#include "stdafx.h"
#include "Client.h"
#include "Protocol.h"
using namespace std;

CClient::CClient() : recvEvent(INVALID_HANDLE_VALUE), receivePacketSize(0)
{

}


CClient::~CClient()
{
	closesocket(mSocket);
}

bool CClient::Init(std::string IP, int PORT)
{
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return false;

	mSocket = socket(AF_INET, SOCK_STREAM, 0);
	ZeroMemory(&addr, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.S_un.S_addr = inet_addr(IP.c_str());

	recvEvent = WSACreateEvent();
	return true;
}



bool CClient::Connect()
{
	addrLen = sizeof(addr);
	if (connect(mSocket, (SOCKADDR*)&addr, addrLen) != 0)
		return false;
	else
	{
		std::cout << "연결 성공!!" << std::endl;
	}
}

bool CClient::SendPacket(CPackets & packet)
{
	retVal = send(mSocket, packet.getPacketBuffer(), packet.getPacketSize(), 0);
	if (retVal == SOCKET_ERROR)
		return false;
	return true;
}

bool CClient::onReceive()
{
	CPackets receivedPacket;
	DWORD bufSize = PACKETBUFFERSIZE - receivePacketSize;
	
	retVal = recv(mSocket, &receiveBuffer[receivePacketSize], bufSize, 0);
	receivePacketSize = retVal;

	while (receivePacketSize > 0)
	{
		receivedPacket.copyToBuffer(receiveBuffer, receivePacketSize);
		if (receivedPacket.isValidPacket() == true && receivePacketSize >= (int)receivedPacket.getPacketSize())
		{

			packetParsing(receivedPacket);
			char buffer[PACKETBUFFERSIZE];
			receivePacketSize -= receivedPacket.getPacketSize();
			CopyMemory(buffer, (receiveBuffer + receivedPacket.getPacketSize()), receivePacketSize);
			CopyMemory(receiveBuffer, buffer, receivePacketSize);
		}
		else
			break;
	}
	return true;
}

void CClient::onClose()
{
	cout << "Error" << endl;

}

void CClient::selectEvent()
{
	WSANETWORKEVENTS netEvent;

	ZeroMemory(&netEvent, sizeof(netEvent));
	WSAEventSelect(mSocket, recvEvent, FD_READ | FD_CLOSE);
	WSAEnumNetworkEvents(mSocket, recvEvent, &netEvent);

	if ((netEvent.lNetworkEvents & FD_READ) == FD_READ)
		onReceive();
	else if ((netEvent.lNetworkEvents & FD_CLOSE) == FD_CLOSE)
		onClose();
}

void CClient::packetParsing(CPackets & packet)
{
	switch (packet.id())
	{
	case  P_CONNECTIONSUCCESS_ACK:	onPTConnectionSuccessAck(packet);		break;
	case  P_TESTPACKET1_ACK:			onPTTestPacket1Ack(packet);			break;
	case  P_TESTPACKET2_ACK:			onPTTestPacket2Ack(packet);			break;
	case  P_TESTPACKET3_ACK:			onPTTestPacket3Ack(packet);			break;
	}
}

void CClient::onPTConnectionSuccessAck(CPackets & packet)
{
	CPackets sendPacket(P_TESTPACKET1_REQ);

	sendPacket << "Test packet 1";
	SendPacket(sendPacket);
}

void CClient::onPTTestPacket1Ack(CPackets & packet)
{
	{
		char str[127];
		packet >> (LPTSTR)str;
		printf("P_TESTPACKET1_ACK received : %s\n", str);
	}

	{
		CPackets sendPacket(P_TESTPACKET2_REQ);

		sendPacket << "Test packet 2";
		SendPacket(sendPacket);
	}
}

void CClient::onPTTestPacket2Ack(CPackets & packet)
{
	{
		char str[127];

		packet >> (LPTSTR)str;
		printf("P_TESTPACKET2_ACK received : %s\n", str);
	}

	{
		CPackets sendPacket(P_TESTPACKET3_REQ);

		sendPacket << "Test packet 3";
		SendPacket(sendPacket);
	}
}

void CClient::onPTTestPacket3Ack(CPackets & packet)
{
	{
		char str[127];

		packet >> (LPTSTR)str;
		printf("P_TESTPACKET1_ACK received : %s\n", str);
	}

	{
		CPackets sendPacket(P_TESTPACKET1_REQ);

		sendPacket << "Test packet 1";
		SendPacket(sendPacket);
	}
}



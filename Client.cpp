#include "stdafx.h"
#include "Client.h"
#include "Protocol.h"
#include "CriticalSections.h"
#include <algorithm>

using namespace std;

CClient::CClient() 
{

}


CClient::~CClient()
{
	shutdown(mSocket, SD_BOTH);
	closesocket(mSocket);
	WSACleanup();
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

	_SelectThread.setSocket(mSocket);

	return true;
}



bool CClient::Connect()
{
	addrLen = sizeof(addr);
	retVal = connect(mSocket, (SOCKADDR*)&addr, addrLen);
	if (retVal == INVALID_SOCKET)
	{
		cout << "Connect()" << endl;
	}
	else
	{
		std::cout << "연결 성공!!" << std::endl;
	}
	return true;
}
	
void CClient::CopyMessageQue()
{
	CCriticalSectionLock cs(_SelectThread.cs);
	recvQue.MessageQue.push(_SelectThread.recvQue.MessageQue.front());
	_SelectThread.recvQue.MessageQue.pop();
}

bool CClient::sendMessage(CPacket packet)
{
	retVal = send(mSocket, packet.getPacketBuffer(), packet.getPacketSize(), 0);
	if (retVal == SOCKET_ERROR)
		return false;
	return true;
}

void CClient::Update()
{
	CCriticalSectionLock cs(cs);
	if (!_SelectThread.recvQue.MessageQue.empty())
	{
		CopyMessageQue();
		packetParsing(recvQue.MessageQue.front());
		recvQue.MessageQue.pop();
	}
	if (!sendQue.MessageQue.empty())
	{
		sendMessage(sendQue.MessageQue.front());
		sendQue.MessageQue.pop();
	}
}

void CClient::packetParsing(CPacket packet)
{
	switch (packet.id())
	{
	case P_CONNECTIONSUCCESS_ACK:		onPConnectionSuccessAck(packet);	break;
	case  P_LOGINPACKET_ACK:			onPTestPacket1Ack(packet);			break;
	case  P_TESTPACKET2_ACK:			onPTestPacket2Ack(packet);			break;
	case  P_TESTPACKET3_ACK:			onPTestPacket3Ack(packet);			break;
	}
}

void CClient::onPConnectionSuccessAck(CPacket & packet)
{
	wchar_t str[127] = { 0, };
	packet >> str;
	printf("%s", str);

	{
		CPacket sendPacket(P_LOGINPACKET_REQ);
		Login log;
		std::cin >> log.ID;
		std::cin >> log.password;
		sendPacket << log;
		sendQue.MessageQue.push(sendPacket);

	}
}
void CClient::onPTestPacket1Ack(CPacket & packet)
{

	{
		wchar_t str[127];
		packet >> str;
		printf("P_TESTPACKET2_ACK received : %s\n", str);
	}


	{
		CPacket sendPacket(P_TESTPACKET2_REQ);
		sendPacket << L"Test packet 2";
		sendQue.MessageQue.push(sendPacket);
	}
}

void CClient::onPTestPacket2Ack(CPacket & packet)
{
	{
		wchar_t str[127];
		packet >> str;
		printf("P_TESTPACKET3_ACK received : %s\n", str);
	}

	{
		CPacket sendPacket(P_TESTPACKET3_REQ);
		sendPacket << L"Test packet 3";
		sendQue.MessageQue.push(sendPacket);
	}
}

void CClient::onPTestPacket3Ack(CPacket & packet)
{
	{
		wchar_t str[127];
		packet >> str;
		printf("P_TESTPACKET3_ACK received : %s\n", str);
	}

	{
		CPacket sendPacket(P_TESTPACKET2_REQ);
		sendPacket << L"Test packet 1";
		sendQue.MessageQue.push(sendPacket);

	}
}



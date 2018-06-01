#include "stdafx.h"
#include "RecvQue.h"


CRecvQue::CRecvQue()
{
}


CRecvQue::~CRecvQue()
{
}

void CRecvQue::packetParsing(CPacket packet)
{
	switch (packet.id())
	{
	case  P_CONNECTIONSUCCESS_ACK:	onPConnectionSuccessAck(packet);		break;
	case  P_TESTPACKET1_ACK:			onPTestPacket1Ack(packet);			break;
	case  P_TESTPACKET2_ACK:			onPTestPacket2Ack(packet);			break;
	case  P_TESTPACKET3_ACK:			onPTestPacket3Ack(packet);			break;
	}
}

void CRecvQue::onPConnectionSuccessAck(CPacket & packet)
{
	{
		wchar_t str[127] = { 0, };
		packet >> str;
		printf("%s", str);
	}
	{

		CPacket sendPacket(P_TESTPACKET1_REQ);

		sendPacket << L"Test packet 1";
		sendQue.push(sendPacket);
	}
}

void CRecvQue::onPTestPacket1Ack(CPacket & packet)
{
	{
		wchar_t str[127];
		packet >> str;
		printf("P_TESTPACKET1_ACK received : %s\n", str);
	}

	{
		CPacket sendPacket(P_TESTPACKET2_REQ);

		sendPacket << L"Test packet 2";
		sendQue.push(sendPacket);
		
	}
}

void CRecvQue::onPTestPacket2Ack(CPacket & packet)
{
	{
		wchar_t str[127];

		packet >> str;
		printf("P_TESTPACKET2_ACK received : %s\n", str);
	}

	{
		CPacket sendPacket(P_TESTPACKET3_REQ);

		sendPacket << L"Test packet 3";
		sendQue.push(sendPacket);
		
	}
}

void CRecvQue::onPTestPacket3Ack(CPacket & packet)
{
	{
		wchar_t str[127];

		packet >> str;
		printf("P_TESTPACKET1_ACK received : %s\n", str);
	}

	{
		CPacket sendPacket(P_TESTPACKET1_REQ);

		sendPacket << L"Test packet 1";
		sendQue.push(sendPacket);
		
	}
}

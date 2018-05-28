#include "stdafx.h"
#include "MessageQueue.h"

void CMessageQueue::packetParsing(CPacket packet)
{

	switch (packet.id())
	{
	case  P_CONNECTIONSUCCESS_ACK:	onPConnectionSuccessAck(packet);		break;
	case  P_TESTPACKET1_ACK:			onPTestPacket1Ack(packet);			break;
	case  P_TESTPACKET2_ACK:			onPTestPacket2Ack(packet);			break;
	case  P_TESTPACKET3_ACK:			onPTestPacket3Ack(packet);			break;
	}
}


void CMessageQueue::onPConnectionSuccessAck(CPacket & packet)
{
	{
		wchar_t str[127] = { 0, };
		packet >> str;
		printf("%s", str);
	}
	{

		CPacket sendPacket(P_TESTPACKET1_REQ);

		sendPacket << L"Test packet 1";
		sendQue.push_back(sendPacket);
		SendPacket();
	}
}

void CMessageQueue::onPTestPacket1Ack(CPacket & packet)
{
	{
		wchar_t str[127];
		packet >> str;
		printf("P_TESTPACKET1_ACK received : %s\n", str);
	}

	{
		CPacket sendPacket(P_TESTPACKET2_REQ);

		sendPacket << L"Test packet 2";
		sendQue.push_back(sendPacket);
		SendPacket();
	}
}

void CMessageQueue::onPTestPacket2Ack(CPacket & packet)
{
	{
		wchar_t str[127];

		packet >> str;
		printf("P_TESTPACKET2_ACK received : %s\n", str);
	}

	{
		CPacket sendPacket(P_TESTPACKET3_REQ);

		sendPacket << L"Test packet 3";
		sendQue.push_back(sendPacket);
		SendPacket();
	}
}

void CMessageQueue::onPTestPacket3Ack(CPacket & packet)
{
	{
		wchar_t str[127];

		packet >> str;
		printf("P_TESTPACKET1_ACK received : %s\n", str);
	}

	{
		CPacket sendPacket(P_TESTPACKET1_REQ);

		sendPacket << L"Test packet 1";
		sendQue.push_back(sendPacket);
		SendPacket();
	}
}




bool CMessageQueue::SendPacket()
{
	for (auto packet : sendQue)
	{
	int	retVal = send(mSocket, packet.getPacketBuffer(), packet.getPacketSize(), 0);
	if (retVal == SOCKET_ERROR)
		return false;
	}
	sendQue.clear();
	return true;
}
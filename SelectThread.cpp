#include "stdafx.h"
#include "SelectThread.h"
#include "CriticalSections.h"

CSelectThread::CSelectThread() : recvEvent(INVALID_HANDLE_VALUE), receivePacketSize(0)
{
	recvEvent = WSACreateEvent();
}


CSelectThread::~CSelectThread()
{
}

void CSelectThread::threadMain()

{	
	while (1)
	{
		Sleep(1);
		WSANETWORKEVENTS netEvent;

		ZeroMemory(&netEvent, sizeof(netEvent));
		WSAEventSelect(mSocket, recvEvent, FD_READ | FD_CLOSE);
		WSAEnumNetworkEvents(mSocket, recvEvent, &netEvent);

		if ((netEvent.lNetworkEvents & FD_READ) == FD_READ)
		{
			onReceive();
		}
		else if ((netEvent.lNetworkEvents & FD_CLOSE) == FD_CLOSE)
			onClose();
	}
}

bool CSelectThread::onReceive()
{

	CPacket receivedPacket;
	DWORD bufSize = PACKETBUFFERSIZE - receivePacketSize;

	retVal = recv(mSocket, &receiveBuffer[receivePacketSize], bufSize, 0);
	receivePacketSize = retVal;
	while (receivePacketSize > 0)
	{
		receivedPacket.copyToBuffer(receiveBuffer, receivePacketSize);
		if (receivedPacket.isValidPacket() == true && receivePacketSize >= (int)receivedPacket.getPacketSize())
		{
			char buffer[PACKETBUFFERSIZE];
			
			CCriticalSectionLock cs(cs);
			recvQue.MessageQue.push(receivedPacket);
			packetParsing(recvQue.MessageQue.front());
			receivePacketSize -= receivedPacket.getPacketSize();
			CopyMemory(buffer, (receiveBuffer + receivedPacket.getPacketSize()), receivePacketSize);
			CopyMemory(receiveBuffer, buffer, receivePacketSize);
		}
		else
			break;
	}
	return true;
}

void CSelectThread::onClose()
{
	std::cout << "error" << std::endl;
}

void CSelectThread::packetParsing(CPacket packet)
{
	
	switch (packet.id())
	{
	case P_CONNECTIONSUCCESS_ACK:		onPConnectionSuccessAck(packet);	break;
	case  P_LOGINPACKET_ACK:			onPSelectLobbyOption(packet);		break;
	case  P_LOBBYOPTION_ACK:			onPSelectLobby(packet);				break;
	case  P_ENTERROOM_ACK:				onPEnterRoom(packet);				break;
	case  P_BROADCAST_ENTER_ROOM_ACK:   onPBroadCastEnterRoom(packet);		break;
	case  P_READY_ACK:					onPReadyAck(packet);				break;
	case  P_READYRESULT_ACK:			onPReadyResultAck(packet);			break;

	}


}

void CSelectThread::onPConnectionSuccessAck(CPacket & packet)
{
	system("cls");
	wchar_t str[127] = { 0, };
	packet >> str;
	printf("%s", str);

	{
		/*CPacket sendPacket(P_LOGINPACKET_REQ);
		Login log;
		std::cin >> log.ID;
		std::cin >> log.password;
		sendPacket << log;
		sendQue.MessageQue.push(sendPacket);*/

	}
}
void CSelectThread::onPSelectLobbyOption(CPacket & packet)
{
	{
		system("cls");
		wchar_t str[127];
		packet >> str;
		printf("%s \n", str);
	}


	{
		/*CPacket sendPacket(P_LOBBYOPTION_REQ);
		int iInsert;
		std::cin >> iInsert;
		sendPacket << iInsert;
		sendQue.MessageQue.push(sendPacket);*/
	}
}


void CSelectThread::onPSelectLobby(CPacket & packet)
{
	{
		system("cls");
		wchar_t str[127];
		packet >> str;
		printf("%s \n", str);
	}


	{
		/*CPacket sendPacket(P_ENTERROOM_REQ);
		int iInput;
		std::cin >> iInput;
		sendPacket << iInput;
		sendQue.MessageQue.push(sendPacket);*/
	}
}

void CSelectThread::onPEnterRoom(CPacket & packet)
{
	{
		system("cls");
		wchar_t str[127];
		packet >> str;
		printf("%s\n", str);
	}

	{
		/*CPacket sendPacket(P_BROADCAST_ENTER_ROOM_REQ);
		sendQue.MessageQue.push(sendPacket);*/

	}
}

void CSelectThread::onPBroadCastEnterRoom(CPacket & packet)
{
	{
		system("cls");
		wchar_t str[127];
		packet >> str;
		printf("%s\n", str);
	}

	{
		/*CPacket sendPacket(P_READY_REQ);
		sendQue.MessageQue.push(sendPacket);*/
	}
}

void CSelectThread::onPReadyResultAck(CPacket & packet)
{
	{
		system("cls");
		wchar_t str[127];
		packet >> str;
		printf("%s\n", str);
	}
}

void CSelectThread::onPReadyAck(CPacket & packet)
{
	{
		std::cout << std::endl;
		wchar_t str[127];
		packet >> str;
		printf("%s\n", str);
	
	}

	{
		/*CPacket sendPacket(P_READYRESULT_REQ);
		int iInput;
		std::cin >> iInput;
		sendPacket << iInput;
		sendQue.MessageQue.push(sendPacket);*/
	}
}
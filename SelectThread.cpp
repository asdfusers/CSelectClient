#include "stdafx.h"
#include "SelectThread.h"
#include "CriticalSections.h"
#include "GameUser.h"
#include "GameManager.h"
#include "Stage.h"
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
			//if (receivedPacket.id() == P_ENEMYPOS_ACK || receivedPacket.id() == P_BROADCAST_READY_ACK)
			//{
			//	GameRecvQue.MessageQue.push(receivedPacket);
			//	GameRecvQue.MessageQue.pop();
			//}
			//else
			//{
				recvQue.MessageQue.push(receivedPacket);
		//	}
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

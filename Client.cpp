#include "stdafx.h"
#include "Client.h"
#include "Protocol.h"
#include "CriticalSections.h"
#include "GameManager.h"
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

void CClient::CopyMessageSendQue()
{
	sendQue.MessageQue.push(_SelectThread.sendQue.MessageQue.front());
	_SelectThread.sendQue.MessageQue.pop();
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
	if (!_SelectThread.recvQue.MessageQue.empty())
	{
		CopyMessageQue();
	}
	CCriticalSectionLock cs(cs);
	if (!recvQue.MessageQue.empty())
	{
		packetSend(recvQue.MessageQue.front().id());
	}

	if (!sendQue.MessageQue.empty())
	{
		sendMessage(sendQue.MessageQue.front());
		sendQue.MessageQue.pop();
	}
}

void CClient::packetSend(unsigned short _packetHeader)
{
	
	switch (_packetHeader)
	{
		case P_CONNECTIONSUCCESS_ACK:
		{
			CPacket sendPacket(P_LOGINPACKET_REQ);
			Login log;
			std::cin >> log.ID;
			std::cin >> log.password;
			sendPacket << log;
			sendQue.MessageQue.push(sendPacket);
		}
		break;
		case P_LOGINPACKET_ACK:
		{
			CPacket sendPacket(P_LOBBYOPTION_REQ);
			int iInsert;
			cin >> iInsert;
			sendPacket << iInsert;
			sendQue.MessageQue.push(sendPacket);
		}
		break;
		case P_LOBBYOPTION_ACK:
		{
			CPacket sendPacket(P_ENTERROOM_REQ);
			int iInput;
			cin >> iInput;
			sendPacket << iInput;
			sendQue.MessageQue.push(sendPacket);
		}
		break;
		case P_ENTERROOM_ACK:
		{
			CPacket sendPacket(P_BROADCAST_ENTER_ROOM_REQ);
			sendQue.MessageQue.push(sendPacket);
		}
		break;
		case P_BROADCAST_ENTER_ROOM_ACK:
		{
			CPacket sendPacket(P_READY_REQ);
			sendQue.MessageQue.push(sendPacket);

		}
		break;
		case P_READY_ACK:
		{
			CPacket sendPacket(P_READYRESULT_REQ);
			int iInput;
			cin >> iInput;
			sendPacket << iInput;
			sendQue.MessageQue.push(sendPacket);
		}
		break;
		case P_READYRESULT_ACK:
		{
			CPacket sendPacket(P_GAMESTARTREADY_REQ);
			sendQue.MessageQue.push(sendPacket);
		}
		break;
		case P_GAMESTARTREADY_ACK:
		{
			CPacket sendPacket(P_GAMESTART_REQ);
			sendQue.MessageQue.push(sendPacket);
		}
		break;
		case P_GAMESTART_ACK:
		{
			CPacket sendPacket(P_GAMEINPUT_REQ);
			
			cInput = _getch();
			CPosition pos = CGameManager::GetInst()->findUser(1)->second.GetPlayerPos();
			sendPacket << cInput << pos;
			sendQue.MessageQue.push(sendPacket);
		}
		break;
		case P_GAMEINPUT_ACK:
		{
			CPacket sendPacket(P_GAMEINPUT_REQ);
			cInput = _getch();
			CPosition pos = CGameManager::GetInst()->findUser(1)->second.GetPlayerPos();
			sendPacket << cInput << pos;
			sendQue.MessageQue.push(sendPacket);
		}
		break;

		case P_ENEMYPOS_ACK:
		{
			
		}
		break;
	}
	recvQue.MessageQue.pop();
}


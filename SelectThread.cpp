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
			if (receivedPacket.id() == P_ENEMYPOS_ACK)
			{
				GameRecvQue.MessageQue.push(receivedPacket);
				packetParsing(GameRecvQue.MessageQue.front());
				GameRecvQue.MessageQue.pop();
			}
			else
			{
				recvQue.MessageQue.push(receivedPacket);
				packetParsing(recvQue.MessageQue.front());
			}
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
	case  P_GAMESTARTREADY_ACK:			onPGameStartAck(packet);			break;
	case  P_GAMESTART_ACK:				onPGameStart(packet);				break;
	case  P_GAMEINPUT_ACK:				onPGameInput(packet);				break;
	case  P_ENEMYPOS_ACK:				onPEnemyPos(packet);				break;
	}
}

void CSelectThread::onPConnectionSuccessAck(CPacket & packet)
{
	system("cls");
	wchar_t str[127] = { 0, };
	packet >> str;
	printf("%s", str);
}
void CSelectThread::onPSelectLobbyOption(CPacket & packet)
{
	{
		system("cls");
		wchar_t str[127];
		packet >> str;
		printf("%s \n", str);
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
}

void CSelectThread::onPEnterRoom(CPacket & packet)
{
	{
		system("cls");
		wchar_t str[127];
		packet >> str;
		printf("%s\n", str);
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

}
void CSelectThread::onPGameStartAck(CPacket & packet)
{
	{
		system("cls");
		wchar_t str[127];
		packet >> str;
		printf("%s\n", str);
	}
}

void CSelectThread::onPGameStart(CPacket & packet)
{
	{
		int iTeam;
		packet >> iTeam;

		if (!CGameManager::GetInst()->Init())
			return;
		if (iTeam == 1)
		{
			CGameUser Player(0, 0);
			CGameUser EnemyPlayer(19, 19);
			Player.setIteam(1);
			CGameManager::GetInst()->insertPool(1, Player);
			CGameManager::GetInst()->insertPool(2, EnemyPlayer);
		}
		else if (iTeam == 2)
		{
			CGameUser Player(19, 19);
			CGameUser EnemyPlayer(0, 0);
			Player.setIteam(2);
			CGameManager::GetInst()->insertPool(1, Player);
			CGameManager::GetInst()->insertPool(2, EnemyPlayer);
		}
		system("cls");
		CGameManager::GetInst()->getStage()->Render(CGameManager::GetInst()->getStage()->m_Stage, &CGameManager::GetInst()->GetUserPool().find(1)->second, &CGameManager::GetInst()->GetUserPool().find(2)->second);
	}
}

void CSelectThread::onPGameInput(CPacket & packet)
{
		bool bAbleCheck;
		wchar_t cInput[5];
		packet >> bAbleCheck >> cInput;

		//변환 과정
		char* pStr;
		int strSize = WideCharToMultiByte(CP_ACP, 0, cInput, -1, NULL, 0, NULL, NULL);
		pStr = new char[strSize];
		WideCharToMultiByte(CP_ACP, 0, cInput, -1, pStr, strSize, 0, 0);

		std::string asdf;
		for (int i = 0; i < sizeof(pStr); i++)
		{
			asdf += pStr[i];
		}
		char keyInput = asdf[0];

		if (bAbleCheck)
		{
			CGameManager::GetInst()->GetUserPool().find(1)->second.MovePlayer(CGameManager::GetInst()->getStage()->m_Stage,
				&CGameManager::GetInst()->findUser(1)->second, keyInput);
			system("cls");
			CGameManager::GetInst()->getStage()->Render(CGameManager::GetInst()->getStage()->m_Stage,
				&CGameManager::GetInst()->findUser(1)->second, &CGameManager::GetInst()->findUser(2)->second);
		}
}

void CSelectThread::onPEnemyPos(CPacket & packet)
{
	bool bAbleCheck;
	wchar_t cInput[5];
	packet >> bAbleCheck >> cInput;

	//변환 과정
	char* pStr;
	int strSize = WideCharToMultiByte(CP_ACP, 0, cInput, -1, NULL, 0, NULL, NULL);
	pStr = new char[strSize];
	WideCharToMultiByte(CP_ACP, 0, cInput, -1, pStr, strSize, 0, 0);

	std::string asdf;
	for (int i = 0; i < sizeof(pStr); i++)
	{
		asdf += pStr[i];
	}
	char keyInput = asdf[0];

	if (bAbleCheck)
	{
		CGameManager::GetInst()->GetUserPool().find(2)->second.MovePlayer(CGameManager::GetInst()->getStage()->m_Stage,
			&CGameManager::GetInst()->findUser(2)->second, keyInput);
		XTrace(L"%d %d", CGameManager::GetInst()->GetUserPool().find(1)->second.GetPlayerPos().x, CGameManager::GetInst()->GetUserPool().find(1)->second.GetPlayerPos().y);
		system("cls");
		CGameManager::GetInst()->getStage()->Render(CGameManager::GetInst()->getStage()->m_Stage,
			&CGameManager::GetInst()->findUser(1)->second, &CGameManager::GetInst()->findUser(2)->second);
	}
	

}

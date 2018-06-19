#include "stdafx.h"
#include "Client.h"
#include "Protocol.h"
#include "CriticalSections.h"
#include "GameManager.h"
#include "KeyInput.h"
#include "resource.h"
#include "DlgBox.h"
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

	if (!recvQue.MessageQue.empty())
	{
		CCriticalSectionLock cs(cs);
		packetHeader = recvQue.MessageQue.front().id();
		packetParsing(recvQue.MessageQue.front());
		recvQue.MessageQue.pop();
		//packetSend(recvQue.MessageQue.front().id());
	}
	if (Input.keyInput())
	{
		if (packetHeader == P_GAMEINPUT_ACK || packetHeader == P_ENEMYPOS_ACK)
		{
			CPacket sendPacket(P_GAMEINPUT_REQ);
			CPosition pos = CGameManager::GetInst()->findUser(1)->second.GetPlayerPos();
			sendPacket << Input.getKey() << pos;
			sendQue.MessageQue.push(sendPacket);

		}
		else 
		{
			CPacket sendPacket(packetHeader + 1);
			sendPacket << Input.getKey();
			sendQue.MessageQue.push(sendPacket);
		}

	}
	if (!sendQue.MessageQue.empty())
	{
		sendMessage(sendQue.MessageQue.front());
		sendQue.MessageQue.pop();
	}
}

void CClient::Print()
{
	if (!printQue.empty())
	{
		if (!bGame)
		{
			system("cls");
		}
		printf("%s", printQue.front());
		printQue.pop();
	}
}

void CClient::Render()
{
	system("cls");
	CGameManager::GetInst()->getStage()->Render(CGameManager::GetInst()->getStage()->m_Stage, &CGameManager::GetInst()->GetUserPool().find(1)->second, &CGameManager::GetInst()->GetUserPool().find(2)->second);
}

void CClient::packetParsing(CPacket packet)
{
	switch (packet.id())
	{
	case P_CONNECTIONSUCCESS_ACK:		onPConnectionSuccessAck(packet);	break;
	case  P_LOGINPACKET_ACK:			onPSelectLobbyOption(packet);		break;
	case  P_LOBBYOPTION_ACK:			onPSelectLobby(packet);				break;
	case  P_ENTERROOM_ACK:				onPEnterRoom(packet);				break;
	case  P_READYRESULT_ACK:			onPReadyResultAck(packet);			break;
	case  P_GAMESTARTREADY_ACK:			onPGameStartAck(packet);			break;
	case  P_GAMESTART_ACK:				onPGameStart(packet);				break;
	case  P_GAMEINPUT_ACK:				onPGameInput(packet);				break;
	case  P_ENEMYPOS_ACK:				onPEnemyPos(packet);				break;
	}
}

void CClient::onPConnectionSuccessAck(CPacket & packet)
{
	packet >> Buffer;	
	printQue.push(Buffer);

	CDlgBox dlgBox;
	dlgBox.LoginDlg();
	Login log;
	
	wcscpy_s(log.ID, dlgBox.getLog().ID);
	wcscpy_s(log.password, dlgBox.getLog().password);
	
	CPacket sendPacket(P_LOGINPACKET_REQ);
	sendPacket << log;
	sendQue.MessageQue.push(sendPacket);
}

void CClient::onPSelectLobbyOption(CPacket & packet)
{
	packet >> Buffer;
	printQue.push(Buffer);
}


void CClient::onPSelectLobby(CPacket & packet)
{
	packet >> Buffer;
	printQue.push(Buffer);
	CDlgBox dlgBox;
	dlgBox.RoomListDlg();
	int Num;
	Num =  dlgBox.getNumber();
	CPacket sendPacket(P_ENTERROOM_REQ);
	sendPacket << Num;
	sendQue.MessageQue.push(sendPacket);

}
void CClient::onPEnterRoom(CPacket & packet)
{
	packet >> Buffer;
	printQue.push(Buffer);
}

void CClient::onPBroadCastEnterRoom(CPacket & packet)
{
	packet >> Buffer;
	printQue.push(Buffer);
}

void CClient::onPBroadCastReadyRoom(CPacket & packet)
{
	packet >> Buffer;
	printQue.push(Buffer);
}

void CClient::onPReadyResultAck(CPacket & packet)
{
	packet >> Buffer;
	printQue.push(Buffer);


	CPacket sendPacket(P_GAMESTARTREADY_REQ);
	sendQue.MessageQue.push(sendPacket);
}

void CClient::onPReadyAck(CPacket & packet)
{
	packet >> Buffer;
	printQue.push(Buffer);

}
void CClient::onPGameStartAck(CPacket & packet)
{
	packet >> Buffer;
	printQue.push(Buffer);
	CPacket sendPacket(P_GAMESTART_REQ);
	sendQue.MessageQue.push(sendPacket);
}

void CClient::onPGameStart(CPacket & packet)
{
	{
		bGame = true;
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
		
	}
}

void CClient::onPGameInput(CPacket & packet)
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
	}
}

void CClient::onPEnemyPos(CPacket & packet)
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

		
	}


}



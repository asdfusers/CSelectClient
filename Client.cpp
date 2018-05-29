#include "stdafx.h"
#include "Client.h"
#include "Protocol.h"
#include "CriticalSections.h"
#include "ThreadManager.h"
#include <algorithm>

using namespace std;

CClient::CClient() 
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

	_SelectThread.setSocket(mSocket);
	_MessageQue.setSocket(mSocket);
	

	
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
		return true;
	}
	
}
	

void CClient::CopyMessageQue()
{
	CriticalSections::getInstance()->enter();
	_MessageQue.messageQue.assign(_SelectThread.getQue().begin(), _SelectThread.getQue().end());
	CriticalSections::getInstance()->leave();
}



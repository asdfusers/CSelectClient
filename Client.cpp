#include "stdafx.h"
#include "Client.h"


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
	return true;
}



bool CClient::Connect()
{
	addrLen = sizeof(addr);
	if (connect(mSocket, (SOCKADDR*)&addr, addrLen) != 0)
		return false;

	std::cout << "연결 성공!!" << std::endl;
}

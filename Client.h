#pragma once
#include "stdafx.h"

class CClient
{
public:
	CClient();
	~CClient();

	bool Init(std::string IP, int PORT);
	bool Connect();

private:
	SOCKET mSocket;
	SOCKADDR_IN addr;
	int addrLen;
	int retVal;

};


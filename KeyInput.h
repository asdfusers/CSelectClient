#pragma once
#include "Packet.h"
class CKeyInput
{
public:
	CKeyInput();
	~CKeyInput();
	bool keyInput();

	wchar_t getKey() { return key; }

private:
	wchar_t key;

};


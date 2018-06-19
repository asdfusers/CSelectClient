#include "stdafx.h"
#include "KeyInput.h"

#include <conio.h>

CKeyInput::CKeyInput()
{
}


CKeyInput::~CKeyInput()
{
}

bool CKeyInput::keyInput()
{
	if (_kbhit())
	{
		key = _getwch();
		return true;
	}
	return false;
}



#include "stdafx.h"
#include "GameManager.h"

CGameManager* CGameManager::m_pInst = NULL;

CGameManager::CGameManager()
{
}


CGameManager::~CGameManager()
{
}

void CGameManager::insertPool(int i, CGameUser & User)
{
	userPool.insert(std::pair<int, CGameUser>(i, User));
}

std::map<int, CGameUser>::iterator CGameManager::findUser(int i)
{
	itr = userPool.begin();
	while (itr != userPool.end())
	{
		if (itr->first == i)
			break;
		else
			++itr;
	}
	return itr;
}

bool CGameManager::Init()
{
	m_pCurStage = new CStage();
	if (!m_pCurStage->Init())
		return false;


	return true;
}

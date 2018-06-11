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

bool CGameManager::Init()
{
	m_pCurStage = new CStage();
	if (!m_pCurStage->Init())
		return false;


	return true;
}

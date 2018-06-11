#pragma once
#include "stdafx.h"
#include "GameUser.h"
#include "Stage.h"

class CGameManager
{
public:
	CGameManager();
	~CGameManager();

	
	void insertPool(int i, CGameUser& User);
	std::map<int, CGameUser>::iterator findUser(int i);
	static CGameManager* GetInst()
	{
		if (!m_pInst)
			m_pInst = new CGameManager;

		return m_pInst;
	}

	bool Init();
	CStage* getStage() { return m_pCurStage; }
	
	std::map<int, CGameUser> GetUserPool() { return userPool; }

private:
	std::map<int, CGameUser> userPool;
	static CGameManager* m_pInst;
	std::map<int, CGameUser>::iterator itr;
	CStage* m_pCurStage;
};


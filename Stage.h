#pragma once
#include "GameUser.h"
#include "stdafx.h"
#define STAGE_HEIGHT 21
#define STAGE_WIDTH 21

class CStage
{
public:
	CStage();
	~CStage();
	
public:
	char m_Stage[STAGE_HEIGHT][STAGE_WIDTH];

public:
	bool CheckMap(int x, int y);
	char GetStage(int x, int y);
	bool Init();
	void Render(char m_pStage[STAGE_HEIGHT][STAGE_WIDTH], CGameUser* Player, CGameUser* EnemyPlayer);
};


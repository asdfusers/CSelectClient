#include "stdafx.h"
#include "Stage.h"


CStage::CStage()
{
}


CStage::~CStage()
{
}

bool CStage::CheckMap(int x, int y)
{
	if (y < 0 || y >= STAGE_HEIGHT)
		return false;

	else if (x < 0 || x >= STAGE_WIDTH)
		return false;

	else if (m_Stage[y][x] == '0')
		return false;

	return true;
}

char CStage::GetStage(int x, int y)
{
	return m_Stage[x][y];
}

bool CStage::Init()
{
	strcpy_s(m_Stage[0], "11100000000000000000");
	strcpy_s(m_Stage[1], "00111111111100000000");
	strcpy_s(m_Stage[2], "00100010000111111100");
	strcpy_s(m_Stage[3], "01100010000000000100");
	strcpy_s(m_Stage[4], "01000011110001111100");
	strcpy_s(m_Stage[5], "01000000001111000000");
	strcpy_s(m_Stage[6], "01100000001000000000");
	strcpy_s(m_Stage[7], "00100000001111111000");
	strcpy_s(m_Stage[8], "00001110000000001000");
	strcpy_s(m_Stage[9], "01111011111111111000");
	strcpy_s(m_Stage[10], "01000000000000000000");
	strcpy_s(m_Stage[11], "01111110011111100000");
	strcpy_s(m_Stage[12], "00000011110000111110");
	strcpy_s(m_Stage[13], "01111110000000000010");
	strcpy_s(m_Stage[14], "01000000001111111110");
	strcpy_s(m_Stage[15], "01111110011000000000");
	strcpy_s(m_Stage[16], "00000010010000000000");
	strcpy_s(m_Stage[17], "01111110011111000000");
	strcpy_s(m_Stage[18], "01000000000001100000");
	strcpy_s(m_Stage[19], "11000000000000111113");

	return true;
}

void CStage::Render(char m_pStage[STAGE_HEIGHT][STAGE_WIDTH], CGameUser * Player, CGameUser * EnemyPlayer)
{
	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 20; j++)
		{
			if (m_pStage[i][j] == '4')
				std::cout << "¢Í";
			else if (Player->GetPlayerPos().x == j && Player->GetPlayerPos().y == i)
				std::cout << "¡Ú";
			else if (EnemyPlayer->GetPlayerPos().x == j && EnemyPlayer->GetPlayerPos().y == i)
				std::cout << "¡Ù";
			else if (m_pStage[i][j] == '0')
				std::cout << "¡á";
			else if (m_pStage[i][j] == '1')
				std::cout << "  ";
			else if (m_pStage[i][j] == '2')
				std::cout << "¡Ú";
			else if (m_pStage[i][j] == '3')
				std::cout << "¡Ý";
			else if (m_pStage[i][j] == '5')
				std::cout << "¢Ì";
			else if (m_pStage[i][j] == '6')
				std::cout << "¡Ø";
			else if (m_pStage[i][j] == '7')

				std::cout << "¡Þ";

		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "t : ÆøÅº¼³Ä¡ " << std::endl;
	std::cout << "w : À§, s : ¾Æ·¡, a : ¿ÞÂÊ, d : ¿À¸¥ÂÊ, q : Á¾·á : ";
}

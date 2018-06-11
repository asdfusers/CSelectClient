#include "stdafx.h"
#include "GameUser.h"


CGameUser::CGameUser(int x, int y)
{
	pos.x = x;
	pos.y = y;
}


CGameUser::~CGameUser()
{
}

void CGameUser::MoveUp(char Maze[21][21], CGameUser * pPlayer)
{
	--pPlayer->pos.y;
}

void CGameUser::MoveDown(char Maze[21][21], CGameUser * pPlayer)
{
	++pPlayer->pos.y;
}

void CGameUser::MoveRight(char Maze[21][21], CGameUser * pPlayer)
{
	++pPlayer->pos.x;
}

void CGameUser::MoveLeft(char Maze[21][21], CGameUser * pPlayer)
{
	--pPlayer->pos.x;
}

void CGameUser::MovePlayer(char Maze[21][21], CGameUser& pPlayer, char cInput)
{
	switch (cInput)
	{
	case 'w':
	case 'W':
		MoveUp(Maze, &pPlayer);
		break;
	case 's':
	case 'S':
		MoveDown(Maze, &pPlayer);
		break;
	case 'a':
	case 'A':
		MoveLeft(Maze, &pPlayer);
		break;
	case 'd':
	case 'D':
		MoveRight(Maze, &pPlayer);
		break;

	case VK_SPACE:
	{
		CreateBomb(Maze, &pPlayer, &pPlayer.BombQue);
		break;
	}
	case 't':
	case 'T':
	{
		Fire(Maze, &pPlayer, pPlayer.BombQue);
	}
	break;
	}

	XTrace(L"%d %d", pPlayer.GetPlayerPos().x, pPlayer.GetPlayerPos().y);
}

void CGameUser::CreateBomb(char Maze[21][21], CGameUser * pPlayer, std::deque<CBomb>* BombQue)
{
	CBomb bomb;
	bomb.pos.x = pPlayer->pos.x;
	bomb.pos.y = pPlayer->pos.y;

	BombQue->push_back(bomb);

	Maze[bomb.pos.y][bomb.pos.x] = '4';
}

void CGameUser::Fire(char Maze[21][21], CGameUser * pPlayer, std::deque<CBomb> bomb)
{
	CPosition pos;
	pos = pPlayer->pos;
	

	for (auto bomb : bomb)
	{
		Maze[bomb.pos.y][bomb.pos.x] = '1';

		for (int j = 1; j <= 1; ++j)
		{
			if (bomb.pos.y - j >= 0)
			{
				if (Maze[bomb.pos.y - j][bomb.pos.x] == '0')
				{
					Maze[bomb.pos.y - j][bomb.pos.x] = '1';
				}

			}
			if (bomb.pos.y + 1 < 20)
			{
				if (Maze[bomb.pos.y + j][bomb.pos.x] == '0')
				{
					Maze[bomb.pos.y + j][bomb.pos.x] = '1';
				}
			}
			if (bomb.pos.x + j < 20)
			{
				if (Maze[bomb.pos.y][bomb.pos.x + j] == '0')
				{
					Maze[bomb.pos.y][bomb.pos.x + j] = '1';
				}
			}
			if (bomb.pos.x - j >= 0)
			{
				if (Maze[bomb.pos.y][bomb.pos.x - j] == '0')
				{
					Maze[bomb.pos.y][bomb.pos.x - j] = '1';
				}
			}
		}

	}
	bomb.clear();
}

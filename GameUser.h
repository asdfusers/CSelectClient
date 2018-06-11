#pragma once
#include "Protocol.h"
#include "Bomb.h"

class CGameUser
{
public:
	CGameUser(int x, int y);
	~CGameUser();


public:
	void MoveUp(char Maze[21][21], CGameUser* pPlayer);
	void MoveDown(char Maze[21][21], CGameUser* pPlayer);
	void MoveRight(char Maze[21][21], CGameUser* pPlayer);
	void MoveLeft(char Maze[21][21], CGameUser* pPlayer);
	void MovePlayer(char Maze[21][21], CGameUser* pPlayer, char cInput);
	void CreateBomb(char Maze[21][21], CGameUser* pPlayer, std::deque<CBomb>* BombQue);
	void Fire(char Maze[21][21], CGameUser * pPlayer, std::deque<CBomb> bomb);

	void setIteam(int _iTeam) { iTeam = _iTeam; }
	int getIteam() { return iTeam; }
	CPosition GetPlayerPos() { return pos; }

private:
	int iTeam;
	CPosition pos;
	CPosition tBombPos[5];
	std::deque<CBomb> BombQue;
};


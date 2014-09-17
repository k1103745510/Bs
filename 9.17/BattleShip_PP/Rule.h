#pragma once

#include "Network.h"
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <conio.h>

#include "stdafx.h"
#include "Enums.h"
#include "Player.h"

#ifdef _DEBUG
#pragma comment(lib, "BGNL_debug.lib")
#else
#pragma comment(lib, "BGNL.lib")
#endif
#include "ShipData.h"

#define NET							false

#define DRAW						true
#define SETUPADV					true
#define PLAYTIMES					100
#define GAMETYPE					EVN

#define MAX_NUMBER_OF_SHIPS_ALL		700			//배를 배치할 수 있는 최대 경우의 수
#define MAX_NUMBER_OF_SHIPS			MapScale*(MapScale-1)*2
#define MAX_HAVE_SHIPS				5
#define MAX_SHIP_LENGTH				5
#define MIN_SHIP_LENGTH				2
#define ATTACK_CHANCE				2
#define PROBABLITY_ADD				100

#define BLACK						0
#define DARKBLUE					1
#define DARKGREEN					2
#define DARKRED						4
#define DARKYELLOW					6
#define GRAY						7
#define DARKGRAY					8
#define BLUE						9
#define GREEN						10
#define RED							12
#define YELLOW						14
#define WHITE						15

#define BASE_COLOR					BLACK
#define BACK_COLOR					DARKGRAY
#define PONT_COLOR					WHITE

static const std::string TurnAttack	= "Atk";
static const std::string TurnDefend	= "Def";

static const int XStartForPlayer[]	= { 4, 50};
static const int YStartForPlayer[]	= { 6, 6};

static HANDLE hConsole;
static CONSOLE_CURSOR_INFO	ConsoleCursor; // 콘솔커서 정보 구조체

class Rule
{
public:
	Rule();
	~Rule();
	void		Init();
	void		InitForNetwork();
	void		Start();
	void		StartNet();

protected:
	void		DrawHitResChar(int shipSize, int hitRes, int drawType);
	void		InitializeScreen();
	void		Clear();
	int			TurnSwap(int turn){ return abs(turn - 1); }
	Position	Attack(Player* pAtk);
	Position	PlayerAttack();
	Position	AiAttack(Player* pAtk, Player* pDef);
	
	Player*		GetPlayer(int who){ return m_TurnedPlayer[who]; }
	Ship*		GetShipOfPosition(int playerWho, int x, int y);
	Map*		SetupShipsAdv(int playerWho);

	void		CursorMoveTo(int x, int y);
	void		SetHit(Position attackPos, HitResult hitRes, int playerWho);
	void		SetHitForNet(Position attackPos, HitResult hitRes);
	void		SetHitForNet2(Position attackPos, HitResult hitRes);
	int			SetPositionLists(Player* atk, int shipIdx, int idxAccum);
	
	void		PrintHitResult(HitResult hitRes);
	void		PrintMaps( Position attackPos);
	void		PrintMap(int playerWho);
	void		PrintShip(Position attackPos, int playerWho);
	void		PrintString(int x, int y, std::string str, int backColor, int PontColor);

protected:
	Player*		m_TurnedPlayer[2];
	Player*		m_Atk;
	Player*		m_Def;

	int			m_Turn;
	int			m_GameType;
	int			m_IdxForSetNet;
	HitResult	m_WhoDestroy[MAX_HAVE_SHIPS];
	Position	m_PosForSetNet[MAX_HAVE_SHIPS];
	HitResult	m_HitResForSetNet[MAX_HAVE_SHIPS];

	Map*		m_MapPlayer[2];
	Map*		m_HitMapPlayer[2];
	Map*		m_MapForSetupAdv[1000];
};
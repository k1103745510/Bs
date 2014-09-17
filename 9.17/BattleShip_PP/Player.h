#pragma once
 
#include "stdafx.h"
#include "Enums.h"

#include "Map.h"
#include "Ship.h"

#include "Aircraft.h"
#include "BattleShip.h"
#include "Cruiser.h"
#include "Destroyer.h"

#define MAX_NUMBER_OF_SHIPS_ALL		700			//배를 배치할 수 있는 최대 경우의 수
#define MAX_NUMBER_OF_SHIPS			MapScale*(MapScale-1)*2
#define MAX_HAVE_SHIPS				5
#define MAX_SHIP_LENGTH				5


class Player
{
public:
	Player();
	~Player();

	void					PlayerInit();

	void					SetupShips();
	void					DecHasShips(){ m_HasShip--; }
	void					SetPlayerType(nPlayer::PlayerType pt){ m_PlayerType = pt; }
	void					SetHasShip(int num){ m_HasShip = num; }
	void					SetMap(Map* map){ m_Map = map; }
	void					InputPositionGroup(int num, PositionGroup pg){ m_AiPositionList[num] = pg; }
	Position				PlayerAttack();
	Position				RandomAttack();
	HitResult				Defend(Position pos);
	
	int						GetHasShip(){ return m_HasShip; }
	Map*					GetMap(){ return m_Map; }
	Map*					GetEnomyMap(){ return m_EnomyMap; }
	Ship*					GetShip(int num){ return m_MyShips[num]; }
	Ship*					GetEnomyShip(int num){ return m_EnomyShips[num]; }
	PositionGroup			GetPositionList(int num){ return m_AiPositionList[num]; }
	nPlayer::PlayerType		GetPlayerType(){ return m_PlayerType; }
	Position				AiAttack(); 


protected:
	int						SetPositionLists(int shipIdx, int shipAccum);					
	Ship*					m_MyShips[MAX_HAVE_SHIPS];	//자신의 배들을 담는 그릇 .
	Ship*					m_EnomyShips[MAX_HAVE_SHIPS];
	nPlayer::PlayerType		m_PlayerType;
	Map*					m_Map;
	Map*					m_EnomyMap;
	PositionGroup			m_AiPositionList[MAX_NUMBER_OF_SHIPS_ALL];
	int						m_HasShip;
	
	Aircraft				m_Aircraft[1];
	BattleShip				m_BattleShip[1];
	Cruiser					m_Cruiser[1];
	Destroyer				m_Destroyer[2];

	Aircraft				m_EnomyAircraft[1];
	BattleShip				m_EnomyBattleShip[1];
	Cruiser					m_EnomyCruiser[1];
	Destroyer				m_EnomyDestroyer[2];
};


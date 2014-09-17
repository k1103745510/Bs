#include "stdafx.h"
#include "BattleShip.h"


BattleShip::BattleShip()
{
	m_Hp = 4;
	m_Size = 4;
	m_Type = nShip::BATTLESHIP;
	m_Name = "BATTLESHIP";
}


BattleShip::~BattleShip()
{
}

HitResult BattleShip::HitCheck(Position pos)
{
	for (int i = 0; i < m_Size; i++)
	{
		if (m_Pos[i].x == pos.x && m_Pos[i].y == pos.y)
		{
			m_Hp--;
			if(m_Hp == 0)
				return BATTLESHIP_DESTROY;
			else
				return HIT;
		}
	}
	return MISS;
}
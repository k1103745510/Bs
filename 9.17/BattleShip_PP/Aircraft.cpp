#include "stdafx.h"
#include "Aircraft.h"


Aircraft::Aircraft()
{
	m_Hp = 5;
	m_Size = 5;
	m_Type = nShip::AIRCRAFT;
	m_Name = "AIRCRAFT";
}


Aircraft::~Aircraft()
{
}

HitResult Aircraft::HitCheck(Position pos)
{
	for (int i = 0; i < m_Size; i++)
	{
		if (m_Pos[i].x == pos.x && m_Pos[i].y == pos.y)
		{
			m_Hp--;
			if(m_Hp == 0)
				return AIRCRAFT_DESTROY;
			else
				return HIT;
		}
	}
	return MISS;
}
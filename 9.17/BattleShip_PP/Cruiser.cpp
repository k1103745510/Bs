#include "stdafx.h"
#include "Cruiser.h"


Cruiser::Cruiser()
{
	m_Hp = 3;
	m_Size = 3;
	m_Type = nShip::CRUISER;
	m_Name = "CRUISER";
}


Cruiser::~Cruiser()
{
}

HitResult Cruiser::HitCheck(Position pos)
{
	for (int i = 0; i < m_Size; i++)
	{
		if (m_Pos[i].x == pos.x && m_Pos[i].y == pos.y)
		{
			m_Hp--;
			if(m_Hp == 0)
				return CRUISER_DESTROY;
			else
				return HIT;
		}
	}
	return MISS;
}
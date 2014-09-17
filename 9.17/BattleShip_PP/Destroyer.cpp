#include "stdafx.h"
#include "Destroyer.h"


Destroyer::Destroyer()
{
	m_Hp = 2;
	m_Size = 2;
	m_Type = nShip::DESTROYER;
	m_Name = "DESTROYER";
}


Destroyer::~Destroyer()
{
}

HitResult Destroyer::HitCheck(Position pos)
{
	for (int i = 0; i < m_Size; i++)
	{
		if (m_Pos[i].x == pos.x && m_Pos[i].y == pos.y)
		{
			m_Hp--;
			if(m_Hp == 0)
				return DESTROYER_DESTROY;
			else
				return HIT;
		}
	}
	return MISS;
}
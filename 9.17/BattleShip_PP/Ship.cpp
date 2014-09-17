#include "stdafx.h"
#include "Ship.h"

Ship::Ship()
{
}
Ship::~Ship()
{
}

void Ship::ShipInit()
{
	for (int i = 0; i < m_Size; ++i)
	{
		m_Pos[i].x = 0;
		m_Pos[i].y = 0;
	}
	m_Hp = m_Size;
}

void Ship::AddPosition(Position pos)
{
	for (int i = 0; i < m_Size; ++i)
	{
		if (m_Pos[i].x == 0)
		{
			m_Pos[i] = pos;
			return ;
		}
	}

	for(int i = 0; i < m_Size; ++i)
	{
		//��ġ�� ������ �迡 ���ο� ��ġ�� ������ �ʱ�ȭ ��Ű�� ����
		m_Pos[i].x = 0;
	}
	AddPosition(pos);
}

void Ship::AddPosition(char x, char y)
{
	Position pos;
	pos.x = x;
	pos.y = y;
	AddPosition(pos);
}

void Ship::Print()
{
	//printf_s("%s: ", m_Name.c_str());
	for (int i = 0; i < m_Size; i++)
	{
		printf_s("%c%c ", m_Pos[i].x, m_Pos[i].y);
	}
	printf_s("\n");
}
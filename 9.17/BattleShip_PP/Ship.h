#pragma once
 
#include <string>
#include "stdafx.h"
#include "Enums.h"

#define MAX_HAVE_SHIPS			5
#define MAX_SHIP_LENGTH			5

class Ship
{
public:
	Ship();
	~Ship();

	void				ShipInit();

	void				AddPosition(Position pos);
	void				AddPosition(char x, char y);
	void				Print();
	void				SetHP(int hp){ m_Hp = hp; }
	
	int					GetHP(){ return m_Hp; }
	int					GetSize(){ return m_Size; }
	nShip::Type			GetType(){ return m_Type; }
	Position			GetPos(int i){ return m_Pos[i]; }

	virtual HitResult	HitCheck(Position pos){ return NONE; }
	
protected:
	std::string			m_Name;
	nShip::Type			m_Type;
	int					m_Hp;
	int					m_Size;	//it means max_hp

	Position			m_Pos[MAX_SHIP_LENGTH];
};
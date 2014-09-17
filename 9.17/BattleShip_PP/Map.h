#pragma once
 
#include "stdafx.h"
#include "Enums.h"
#include "Math.h"

#define MAX_NUMBER_OF_SHIPS_ALL		700			//�踦 ��ġ�� �� �ִ� �ִ� ����� ��
#define MapScale					8
#define MAX_NUMBER_OF_SHIPS			MapScale*(MapScale-1)*2
#define MAX_HAVE_SHIPS				5
#define MAX_SHIP_LENGTH				5

class Map
{
public:
	Map();
	~Map();

	void			MapInit();

	int				SetArrangablePosition(int size);
	void			AddPositionGroup(PositionGroup PositionGroup);
	PositionGroup*	GetPositionList(){return m_PositionList; }

	int				ShipSize(int x,int y){ return m_ShipSize[x][y]; }
	HitResult		IsHit(int x, int y){ return m_IsHit[x][y]; }
	void			SetShip(Position pos, int size);
	void			SetHit(Position pos, HitResult hitRes);
	
public:
	int				IndexCount;

private:
	int				m_ShipSize[MapScale][MapScale];
	//�谡 ������ 0�� .
	HitResult		m_IsHit[MapScale][MapScale];
	// NONE : no hit, MISS : hit. no ship, HIT : hit. is ship, else : hit. is destroy
	PositionGroup	m_PositionList[MAX_NUMBER_OF_SHIPS];
	//��ġ�ϱ� ��, �踦 ��ġ�� �� �ִ� ����� ���� ��� �׸�
};


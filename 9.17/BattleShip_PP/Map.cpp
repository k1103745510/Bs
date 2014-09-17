#include "stdafx.h"
#include "Map.h"


Map::Map()
{
}

Map::~Map()
{
}

void Map::MapInit()
{
	IndexCount = 0;
	for(int x = 0; x < MapScale; ++x)
	{
		for(int y = 0; y < MapScale; ++y)
		{
			m_IsHit[x][y] = NONE;
			m_ShipSize[x][y] = 0;
		}
	}
}

int Map::SetArrangablePosition(int size)
{
	//특정 Size의 배를 배치할 수 있는 경우의 수을 모두 찾아서 m_ShipsPlace 에 넣는 함수.
	//Arrangable 을 대체할 좀더 명확한 단어가 없을까?
	if(size < 0 || MAX_SHIP_LENGTH < size)
		return -1;
	
	memset(m_PositionList, 0, sizeof(PositionGroup) * MAX_NUMBER_OF_SHIPS);
	PositionGroup ret;
	Math math;

	IndexCount = 0;

	for(int x = 0; x < MapScale; x++)
	{
		for(int y = 0; y < MapScale; y++)
		{
			for(int s = 0; s < size; s++)
			{
				if(ShipSize(x + s, y) == 0 && x + s < MapScale)	//배를 가로로 배치하는 경우
				{
					ret.pos[s].x = math.IntToCharX(x + s);
					ret.pos[s].y = math.IntToCharY(y);
					if(s == size - 1)
					{
						AddPositionGroup(ret);	//배의 크기만큼 입력하는데 성공했으므로 m_PositionList 에 저장하자!
						IndexCount++;
					}
					continue;
				}
				break;
			}

			for(int s = 0; s < size; s++)
			{
				if(ShipSize(x, y + s) == 0 && y + s < MapScale)	//배를 가로로 배치하는 경우
				{
					ret.pos[s].x = math.IntToCharX(x);
					ret.pos[s].y = math.IntToCharY(y + s);
					if(s == size - 1)
					{
						AddPositionGroup(ret);	//배의 크기만큼 입력하는데 성공했으므로 m_PositionList 에 저장하자!
						IndexCount++;
					}
					continue;
				}
				break;
			}
		}
	}
	return IndexCount;
}


void Map::AddPositionGroup(PositionGroup PositionGroup)
{
	if(PositionGroup.pos[0].x == 0)
		return ;

	//PositionList 의 가장 앞에 있는 빈자리에 PositionGroup 을 넣는 함수
	for(int i = 0; i < MAX_NUMBER_OF_SHIPS_ALL; i++)
	{
		if(m_PositionList[i].pos[0].x == 0)
		{
			m_PositionList[i] = PositionGroup;
			return ;
		}
	}
}

void Map::SetShip(Position pos, int size)
{
	Math math;
	int x = math.CharToIntX(pos.x);
	int y = math.CharToIntY(pos.y);
	m_ShipSize[x][y] = size;
}

void Map::SetHit(Position pos, HitResult hitRes)
{
	Math math;
	int x = math.CharToIntX(pos.x);
	int y = math.CharToIntY(pos.y);
	m_IsHit[x][y] = hitRes;
}
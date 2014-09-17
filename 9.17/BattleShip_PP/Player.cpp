#include "stdafx.h"
#include "Player.h"

Player::Player()
{
	m_HasShip = MAX_HAVE_SHIPS;

	m_Map = new Map();
	m_EnomyMap = new Map();

	m_MyShips[0] = &m_Aircraft[0];
	m_MyShips[1] = &m_BattleShip[0];
	m_MyShips[2] = &m_Cruiser[0];
	m_MyShips[3] = &m_Destroyer[0];
	m_MyShips[4] = &m_Destroyer[1];

	m_EnomyShips[0] = &m_EnomyAircraft[0];
	m_EnomyShips[1] = &m_EnomyBattleShip[0];
	m_EnomyShips[2] = &m_EnomyCruiser[0];
	m_EnomyShips[3] = &m_EnomyDestroyer[0];
	m_EnomyShips[4] = &m_EnomyDestroyer[1];
}
Player::~Player()
{
	delete m_Map;
	m_Map = nullptr;
	delete m_EnomyMap;
	m_EnomyMap = nullptr;
}

void Player::PlayerInit()
{
	m_HasShip = MAX_HAVE_SHIPS;
	for(int i = 0; i < MAX_HAVE_SHIPS; ++i)
	{
		m_MyShips[i]->ShipInit();
		m_EnomyShips[i]->ShipInit();
	}

	m_Map->MapInit();
	m_EnomyMap->MapInit();
}

void Player::SetupShips()
{
	//�踦 ������ġ�ϴ� �Լ�
	for(int shipCnt = 0; shipCnt < MAX_HAVE_SHIPS; shipCnt++)
	{
		Map* pMap = GetMap();
		Ship* pShip = GetShip(shipCnt);
		int size = pShip->GetSize();

		pMap->SetArrangablePosition(size);
		//PositionList �� ���� ũ�⺰�� �� �� �ִ� ����� ���� �Է���
		PositionGroup* PositionList = pMap->GetPositionList();

		int cmd = rand() % (pMap->IndexCount - 1);
		//�� ����� �� �� �ϳ��� ������
		for(int k = 0; k < size; k++)
		{
			//shipCnt ��° ���� ũ�⸸ŭ �ݺ��ϰ���
			Position pos = PositionList[cmd].pos[k];
			pMap->SetShip(pos, size);
			pShip->AddPosition(pos);
		}
	}
}

Position Player::PlayerAttack()
{
	Position attackPos = {0, 0};
	HitResult hitRes;
	int x, y;
	int first = 0;

	do
	{
		if(0 < first)
		{
			printf_s("\n Insert Valid Position : ");
			if(first == ATTACK_CHANCE)
			{
				printf_s(" You Lose Turn. lol");
				attackPos = RandomAttack();
				break;
			}
		}
		first++;
		
		Position pos;
		pos.x = _getche();
		pos.y = _getche();
		attackPos = pos;

		if(attackPos.x < 'A' || 'A' + MapScale <= attackPos.x)
			attackPos.x += ('A' - 'a');
		//��ҹ��� ��������
			
		Math math;
		x = math.CharToIntX(attackPos.x);
		y = math.CharToIntY(attackPos.y);
		hitRes = GetEnomyMap()->IsHit(x, y);

	}while(( x<0 || MapScale<=x ) || ( y<0 || MapScale<=y ) || ( hitRes != NONE ));

	return attackPos;
}

Position Player::RandomAttack()
{
	Position ret = {0, 0};
	Math math;
	for(int x = 0; x < MapScale; x++)
	{
		for(int y = 0; y < MapScale; y++)
		{
			if(GetEnomyMap()->IsHit(x, y) == NONE)
			{
				ret.x = math.IntToCharX(x);
				ret.y = math.IntToCharY(y);
				return ret;
			}
		}
	}
	return ret;
}

HitResult Player::Defend(Position pos)
{
	//�� �� pos �� ��ȿ�� ���̾����( ���ݵ��� �ʰ�, �������� ����� �ʴ� ���� )
	for(int i = 0; i < MAX_HAVE_SHIPS; i++)
	{
		HitResult ret = GetShip(i)->HitCheck(pos);
		if(ret != MISS)
		{
			if(ret != HIT)
				DecHasShips();
			return ret;
		}
	}
	return MISS;
}

Position Player::AiAttack()
{
	//AI �κ��� �� �ϼ��ϰ� �� �� �ڵ��� �˾ƺ� �� �ֵ��� �����ľ߰��� .
	Math math;
	int idxCountAccum = 0;
	int sizeCutIndex = 0;
	PositionGroup pg;
	Position pos;
	Ship* pShip;
	
	int sizeCut[MAX_SHIP_LENGTH];
	//�� ���� ��ġ�� �� �ִ� ����� ���� ������ �����ϴ� �迭 .
	//�� ���� m_AiPositionList �� �̿��Ͽ� ��� ������ �� ���� �� �� .
	for(int i = 0; i < MAX_SHIP_LENGTH; ++i)
	{
		pg.pos[i].x = 0;
		pg.pos[i].y = 0;
	}

	for(int i = 0; i < MAX_NUMBER_OF_SHIPS_ALL; ++i)	// �ʱ�ȭ
	{
		InputPositionGroup(i, pg);
	}

	for(int i = 0; i < MAX_HAVE_SHIPS; ++i)
	{
		pShip = GetEnomyShip(i);
		if(pShip->GetHP() != 0)
		{
			sizeCut[sizeCutIndex] = SetPositionLists(pShip->GetSize(), idxCountAccum);
			//SetPositionLists �� ��ġ�� �� �ִ� ���� ���� �������ݴϴ� .
			idxCountAccum += sizeCut[sizeCutIndex++];
		}
	}
	//�� ���� ��ġ�� �� �ִ� ����� ������ ������ ������
	//m_AiPositionList[ sizeCut[i - 1] ~ sizeCut[i] ] ��  �谡 �� �� �ִ� ��ǥ���� ����Ǿ� ����
	//sizeCut[i] - sizeCut[i - 1] �� �� �谡 ������ �� �ִ� ����� ����

	int firstIdx;
	int CanAllShipPosition[MapScale][MapScale];
	for(int x = 0; x < MapScale; ++x)
	{
		for(int y = 0; y < MapScale; ++y)
		{
			CanAllShipPosition[x][y] = 0;
		}
	}

	for(int i = 0; i < sizeCutIndex; i++)
	{
		int CanIsShipPosition[MapScale][MapScale];
		for(int x = 0; x < MapScale; ++x)
		{
			for(int y = 0; y < MapScale; ++y)
			{
				CanIsShipPosition[x][y] = 0;
			}
		}

		if(i == 0)
			firstIdx = 0;
		else
			firstIdx = sizeCut[i - 1];

		int lastIdx = firstIdx + sizeCut[i];

		for(int pI = firstIdx; pI < lastIdx; ++pI)
		{
			int probAdd = 1;
			for(int num = 0; num < MAX_SHIP_LENGTH; ++num)
			{
				pos = GetPositionList(pI).pos[num];
				if(pos.x != 0)
				{
					int x = math.CharToIntX(pos.x);
					int y = math.CharToIntY(pos.y);
					if(GetEnomyMap()->IsHit(x,y) == HIT)
					{
						if(probAdd > PROBABLITY_ADD)
							probAdd += 5 * PROBABLITY_ADD;
						probAdd += PROBABLITY_ADD;
						//Ư�� ��ǥ�� HIT ���¸� �� ��ǥ�� �����ϴ� ����� ����(��ǥ�׷����� �� ���)�� ����� ����
					}
				}
			}

			for(int num = 0; num < MAX_SHIP_LENGTH; ++num)
			{
				pos = GetPositionList(pI).pos[num];
				if(pos.x != 0)
				{
					int x = math.CharToIntX(pos.x);
					int y = math.CharToIntY(pos.y);
					if(GetEnomyMap()->IsHit(x,y) != HIT)
					{
						CanIsShipPosition[x][y]++;
						CanAllShipPosition[x][y] += probAdd;
					}
				}
			}
		}

		for(int x = 0; x < MapScale; ++x)
		{
			for(int y = 0; y < MapScale; ++y)
			{
				if(CanIsShipPosition[x][y] == sizeCut[i])	//Ȯ���� 100% �� ���
				{
					Position ret;
					ret.x = math.IntToCharX(x);
					ret.y = math.IntToCharY(y);
					return ret;
				}
			}
		}
	}

	Position ret;
	int maxProb = 0;
	int _x, _y;
	for(int x = 0; x < MapScale; ++x)
	{
		for(int y = 0; y < MapScale; ++y)
		{
			if(maxProb < CanAllShipPosition[x][y])
			{
				//�踦 ��ġ�� �� �ִ� ����� ���� ���� ���� ������ ����!
				maxProb = CanAllShipPosition[x][y];
				_x = x;
				_y = y;
			}
		}
	}
	ret.x = math.IntToCharX(_x);
	ret.y = math.IntToCharY(_y);
	return ret;
}

int Player::SetPositionLists(int size, int idxAccum)
{
	if(size < 0 || MAX_SHIP_LENGTH < size)
		return -1;

	PositionGroup ret = {0, };
	// m_AiPositionList �� ��ġ�� �� �ִ� ����� ���� �����ϴ� �Լ� .
	int idxCount = 0;
	Math math;

	for(int x = 0; x < MapScale; ++x)
	{
		for(int y = 0; y < MapScale; ++y)
		{
			for(int s = 0; s < size; ++s)
			{
				HitResult hitRes = GetEnomyMap()->IsHit(x + s, y);
				if(( hitRes == NONE || hitRes == HIT ) && x + s < MapScale)	// �� ��ǥ�� �谡 ���� ���ɼ��� �ִٸ�
				{
					ret.pos[s].x = math.IntToCharX(x + s);
					ret.pos[s].y = math.IntToCharY(y);
					if(s == size - 1)
					{
						InputPositionGroup(idxCount + idxAccum, ret);
						idxCount++;
					}
					continue;
				}
				break;
			}

			for(int s = 0; s < size; s++)
			{
				HitResult hitRes = GetEnomyMap()->IsHit(x, y + s);
				if(( hitRes == NONE || hitRes == HIT ) && y + s < MapScale)
				{
					ret.pos[s].x = math.IntToCharX(x);
					ret.pos[s].y = math.IntToCharY(y + s);
					if(s == size - 1)
					{
						InputPositionGroup(idxCount + idxAccum, ret);
						idxCount++;
					}
					continue;
				}
				break;
			}
		}
	}
	return idxCount;
}
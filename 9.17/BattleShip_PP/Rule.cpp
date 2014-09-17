#include "stdafx.h"
#include "Rule.h"

Rule::Rule(void)
{
	m_TurnedPlayer[0] = new Player();
	m_TurnedPlayer[1] = new Player(); 
	m_HitMapPlayer[0] = new Map();
	m_HitMapPlayer[1] = new Map();
	m_MapPlayer[0] = new Map();
	m_MapPlayer[1] = new Map();
}
Rule::~Rule(void)
{
	delete m_TurnedPlayer[0];
	m_TurnedPlayer[0] = nullptr;
	delete m_TurnedPlayer[0];
	m_TurnedPlayer[1] = nullptr;
	delete m_HitMapPlayer[0];
	m_HitMapPlayer[0] = nullptr;
	delete m_HitMapPlayer[1];
	m_HitMapPlayer[1] = nullptr;
	delete m_HitMapPlayer[0];
	m_MapPlayer[0] = nullptr;
	delete m_HitMapPlayer[1];
	m_MapPlayer[1] = nullptr;
}

void Rule::Init()
{
	InitializeScreen();

	m_Turn = 0;
	//First Attacker is Player 1
	m_Atk = m_TurnedPlayer[m_Turn];
	m_Def = m_TurnedPlayer[TurnSwap(m_Turn)];

	m_GameType = GAMETYPE;
	switch(m_GameType)
	{
	case PVP:
		m_Atk->SetPlayerType(nPlayer::NORMAL);
		m_Def->SetPlayerType(nPlayer::NORMAL);
		break;
	case PVE:
		m_Atk->SetPlayerType(nPlayer::NORMAL);
		m_Def->SetPlayerType(nPlayer::AI);
		break;
	case EVE:
		m_Atk->SetPlayerType(nPlayer::AI);
		m_Def->SetPlayerType(nPlayer::AI);
		break;
	case EVN:
		m_Atk->SetPlayerType(nPlayer::AI);
		m_Def->SetPlayerType(nPlayer::NONE);
		break;
	default:
		break;
	}

	m_Atk->PlayerInit();
	m_Def->PlayerInit();
	m_MapPlayer[m_Turn]->MapInit();
	m_MapPlayer[TurnSwap(m_Turn)]->MapInit();
	m_HitMapPlayer[m_Turn]->MapInit();
	m_HitMapPlayer[TurnSwap(m_Turn)]->MapInit();

	if(SETUPADV)
	{
		Map* map0;
		Map* map1;
		map0 = SetupShipsAdv(0);
		
		m_MapPlayer[m_Turn]->MapInit();
		m_MapPlayer[TurnSwap(m_Turn)]->MapInit();
		m_HitMapPlayer[m_Turn]->MapInit();
		m_HitMapPlayer[TurnSwap(m_Turn)]->MapInit();
		
		map1 = SetupShipsAdv(1);
		
		m_MapPlayer[m_Turn]->MapInit();
		m_MapPlayer[TurnSwap(m_Turn)]->MapInit();
		m_HitMapPlayer[m_Turn]->MapInit();
		m_HitMapPlayer[TurnSwap(m_Turn)]->MapInit();
		
		m_Atk->SetMap(map0);
		m_Def->SetMap(map1);
	}
	else
	{
		m_Atk->SetupShips();
		m_Def->SetupShips();
	}

	m_MapPlayer[m_Turn] = m_Atk->GetMap();
	m_MapPlayer[TurnSwap(m_Turn)] = m_Def->GetMap();
}

void Rule::InitForNetwork()
{
	InitializeScreen();

	m_Atk = m_TurnedPlayer[0];
	m_Def = m_TurnedPlayer[1];
	
	m_GameType = GAMETYPE;
	m_Atk->SetPlayerType(nPlayer::AI);
	m_Def->SetPlayerType(nPlayer::NONE);

	m_Atk->PlayerInit();
	m_Def->PlayerInit();
	m_MapPlayer[0]->MapInit();
	m_HitMapPlayer[0]->MapInit();
	m_MapPlayer[1]->MapInit();
	m_HitMapPlayer[1]->MapInit();

	//m_Atk->SetMap(SetupShipsAdv());
	m_Atk->SetupShips();

	m_MapPlayer[0] = m_Atk->GetMap();
	m_MapPlayer[1] = m_Def->GetMap();
}

void Rule::Start()
{
	if(NET)
	{
		StartNet();
	}
	else
	{
		Init();

		//CString BGM = "res\\test.wav";
		HitResult hitRes = NONE;
		Position attackPos = {0, 0 };
		int count = 0;
		double sumCount = 0;
		for (int i = 0; i < PLAYTIMES; ++i) 
		{
			do
			{
				if(DRAW)
					PrintMaps(attackPos);
				attackPos = Attack(m_Atk);
				hitRes = m_Def->Defend(attackPos);
				SetHit(attackPos, hitRes, m_Turn);
			
				count += i % 2;	// Ȧ�� �ϸ��� �� ����
				if(DRAW)
				{
					Clear();
					PrintHitResult(hitRes);
					printf("�� �� : %d", count);
				}

				if(m_GameType != EVN)
					m_Turn = TurnSwap(m_Turn);
				m_Atk = m_TurnedPlayer[m_Turn];
				m_Def = m_TurnedPlayer[TurnSwap(m_Turn)];

			}while(( m_TurnedPlayer[0]->GetHasShip() != 0 ) &&
				( m_TurnedPlayer[1]->GetHasShip() != 0 ));
			
			if(DRAW)
			{
				PrintMaps(attackPos);
				getchar();
			}
			Init();
			sumCount += count;
			count = 0;
		}
		sumCount = sumCount / PLAYTIMES;
		printf("��� Ƚ�� : %f\n", sumCount);
	}
	/*
	Init();

	Map* pMap = m_Def->GetMap();
	Ship* pShip;

	PositionGroup* PositionList;
	PositionGroup pg[MAX_HAVE_SHIPS];
	Position pos;
	Position attackPos = {0, };
	HitResult hitRes;

	int arr[MAX_HAVE_SHIPS];
	int idx = 0;
	int idxCounter = -1;
	int lastSet = 0;
	int mapIndexCount = 0;
	int mapIdxs[1000];
	int count = 0;
	int maxCount = 0;
	int size;
		
	m_Def->PlayerInit();

	for(int i = 0; i < MAX_HAVE_SHIPS; ++i)
	{
		arr[i] = 0;
	}

	while(true)
	{
		//�� while ���� Max_Have_Ships ��ŭ ��ø�� for��ó�� �� �� ���� .
		//�׸��� �� ���� Ƚ���� lastSet �� .
		if(arr[idx] <= lastSet)
		{
			if(idx < MAX_HAVE_SHIPS)
			{
				//������ ����� ��ġ���� �ʾҴٸ� �踦 �ϳ��� ��ġ��
				if(idxCounter != idx)
				{
					pShip = m_Def->GetShip(idx);
					size = pShip->GetSize();

					pMap->SetArrangablePosition(size);
					lastSet = pMap->IndexCount - 1;
					//lastSet�� ���� ���¿��� idx��° ���� ��ġ ������ ����� ���� .
					PositionList = pMap->GetPositionList();
					idxCounter = idx;
				}

				for(int i = 0; i < size; ++i)	//idx ��° �踦 ��ġ��
				{
					pg[idx] = *(PositionList + arr[idx]);
					pos = pg[idx].pos[i];
					pMap->SetShip(pos, size);
					pShip->AddPosition(pos);
				}
				//���� �踦 ��ġ�ϱ� ���Ͽ� idx ���� 1 �ø� .
				idx++;

				if(idx == MAX_HAVE_SHIPS)
				{
					idx--;
					//������ ����� ��ġ������ ,
					//���� ���¿��� ���� �������� �ɸ� Ƚ�� �迭�� ����
					m_MapPlayer[1] = pMap;

					do
					{
						if(DRAW)
							PrintMaps(attackPos);
						attackPos = Attack(m_Atk);
						hitRes = m_Def->Defend(attackPos);
						SetHit(attackPos, hitRes);
			
						count += TurnSwap(m_Turn);
						if(DRAW)
						{
							Clear();
							PrintHitResult(hitRes);
							printf("�� �� : %d", count);
						}
					}while(( m_TurnedPlayer[0]->GetHasShip() != 0 ) &&
						( m_TurnedPlayer[1]->GetHasShip() != 0 ));

					//PrintMaps(attackPos);
					//printf("�� �� : %d", count);
					//getchar();

					m_Atk->GetEnomyMap()->MapInit();
					m_HitMapPlayer[1]->MapInit();
					m_Def->SetHasShip(MAX_HAVE_SHIPS);
					for(int i = 0; i < MAX_HAVE_SHIPS; ++i)
					{
						Ship* ship = m_Def->GetShip(i);
						ship->SetHP(ship->GetSize());
						m_Atk->GetEnomyShip(i)->ShipInit();
					}

					if(idx <= 2)
					{
						PrintMaps(attackPos);
					}
					
					if(count >= 45)
					{
						PrintMaps(attackPos);
						CursorMoveTo(0,0);
						printf("�� �� : %d\n", count);
						printf("%d %d %d %d %d\n",arr[0],arr[1],arr[2],arr[3],arr[4]);
						printf("ã�� �� : %d\n", mapIndexCount);
						m_MapForSetupAdv[mapIndexCount] = pMap;
						mapIdxs[mapIndexCount] = count;
						mapIndexCount++;
						//getchar();
					}
						
					count = 0;
						
					pShip = m_Def->GetShip(idx);
					size = pShip->GetSize();
					for(int i = 0; i < size; ++i)
					{
						pos = pg[idx].pos[i];
						pMap->SetShip(pos, 0);
					}	//���� �������� ��ġ�� �� ����

					arr[idx]++;
					//������ �踦 ���� ��ġ�� ��ġ�ϱ� ���ؼ� arr[idx] ���� 1 �ø� .
				}
			}
		}
		else
		{
			idx--;
			//�踦 ��ġ�ϴ� ��, idx ��° �踦 ������ ��ġ���� ��ġ�غ�����,
			//idx - 1 ��° �踦 �ٽ� ��ġ�ϱ� ���ؼ�,
			m_Atk->GetEnomyMap()->MapInit();
			m_HitMapPlayer[1]->MapInit();
			m_Def->SetHasShip(MAX_HAVE_SHIPS);
			for(int i = 0; i < MAX_HAVE_SHIPS; ++i)
			{
				Ship* ship = m_Def->GetShip(i);
				ship->SetHP(ship->GetSize());
				m_Atk->GetEnomyShip(i)->ShipInit();
			}
				
			pShip = m_Def->GetShip(idx);
			size = pShip->GetSize();
			for(int i = 0; i < size; ++i)	//���� �������� ��ġ�� �� �����ϸ�,
			{
				pos = pg[idx].pos[i];
				pMap->SetShip(pos, 0);
			}

			arr[idx + 1] = 0;
			arr[idx]++;
			if(idx == 0)	//������ ���� ��츦 �ǹ��ϹǷ� Ż���Ѵ�.
				break;
		}
	}
	*/
}

void Rule::StartNet()
{
	try
	{
		Network::Initialize();
	}
	catch (Network::Exception)
	{
		puts("�ʱ�ȭ ���� ������ �߻��߽��ϴ�.");
		return;
	}

	Math math;
	Network network;
	ShipData shipData;
	ErrorType error;
	char mapData[MapScale*MapScale];
	PacketType type;
	Position attackPos;
	Coord coord[16];
	Coord attackCoord;
		
	bool over = false;
	bool allOver = false;
	int x, y;
	m_IdxForSetNet = 0;

	try
	{
		network.Connect("10.73.42.117", 9000);
		error = network.SubmitName(L"OxO", 141017);
		if (error == ET_DUPLICATED_NAME)
		{
			puts("�̹� �����ϴ� �̸��Դϴ�.");
			return;
		}
		puts("��븦 ��ٸ��� ��...\n");
		Network::GameStartData gameStartData;
		network.WaitForStart(&gameStartData);
		wprintf_s(L"��Ī�Ǿ����ϴ�. ���� �̸�: %s, �й�: %d\n", gameStartData.oppositionName, gameStartData.oppositionStudentID);
			
		while(!allOver)
		{
			InitForNetwork();
			#pragma region InsertShipData
					
			int ship = 0;
			int pos = 0;
			for(int i = 0; i < 16; i++)
			{
				coord[i] = Coord(m_Atk->GetShip(ship)->GetPos(pos).x,
					m_Atk->GetShip(ship)->GetPos(pos).y);
				pos++;
				switch (ship)
				{
				case 0:
					if(pos >= 5)
					{
						pos = 0;
						ship++;
					}
					break;
				case 1:
					if(pos >= 4)
					{
						pos = 0;
						ship++;
					}
					break;
				case 2:
					if(pos >= 3)
					{
						pos = 0;
						ship++;
					}
					break;
				case 3:
					if(pos >= 2)
					{
						pos = 0;
						ship++;
					}
					break;
				case 4:
					if(pos >= 2)
					{
						pos = 0;
						ship++;
					}
					break;
				default:
					break;
				}
			}
			/*
			coord[0].mX = m_Atk->GetShip(0)->GetPos(0).x;
			coord[0].mY = m_Atk->GetShip(0)->GetPos(0).y;
			coord[1].mX = m_Atk->GetShip(0)->GetPos(1).x;
			coord[1].mY = m_Atk->GetShip(0)->GetPos(1).y;
			coord[2].mX = m_Atk->GetShip(0)->GetPos(2).x;
			coord[2].mY = m_Atk->GetShip(0)->GetPos(2).y;
			coord[3].mX = m_Atk->GetShip(0)->GetPos(3).x;
			coord[3].mY = m_Atk->GetShip(0)->GetPos(3).y;
			coord[4].mX = m_Atk->GetShip(0)->GetPos(4).x;
			coord[4].mY = m_Atk->GetShip(0)->GetPos(4).y;

			coord[5].mX = m_Atk->GetShip(1)->GetPos(0).x;
			coord[5].mY = m_Atk->GetShip(1)->GetPos(0).y;
			coord[6].mX = m_Atk->GetShip(1)->GetPos(1).x;
			coord[6].mY = m_Atk->GetShip(1)->GetPos(1).y;
			coord[7].mX = m_Atk->GetShip(1)->GetPos(2).x;
			coord[7].mY = m_Atk->GetShip(1)->GetPos(2).y;
			coord[8].mX = m_Atk->GetShip(1)->GetPos(3).x;
			coord[8].mY = m_Atk->GetShip(1)->GetPos(3).y;

			coord[9].mX = m_Atk->GetShip(2)->GetPos(0).x;
			coord[9].mY = m_Atk->GetShip(2)->GetPos(0).y;
			coord[10].mX = m_Atk->GetShip(2)->GetPos(1).x;
			coord[10].mY = m_Atk->GetShip(2)->GetPos(1).y;
			coord[11].mX = m_Atk->GetShip(2)->GetPos(2).x;
			coord[11].mY = m_Atk->GetShip(2)->GetPos(2).y;

			coord[12].mX = m_Atk->GetShip(3)->GetPos(0).x;
			coord[12].mY = m_Atk->GetShip(3)->GetPos(0).y;
			coord[13].mX = m_Atk->GetShip(3)->GetPos(1).x;
			coord[13].mY = m_Atk->GetShip(3)->GetPos(1).y;

			coord[14].mX = m_Atk->GetShip(4)->GetPos(0).x;
			coord[14].mY = m_Atk->GetShip(4)->GetPos(0).y;
			coord[15].mX = m_Atk->GetShip(4)->GetPos(1).x;
			coord[15].mY = m_Atk->GetShip(4)->GetPos(1).y;
			*/
			shipData.SetShipCoord(MD_AIRCRAFT, 0, coord[0]);
			shipData.SetShipCoord(MD_AIRCRAFT, 1, coord[1]);
			shipData.SetShipCoord(MD_AIRCRAFT, 2, coord[2]);
			shipData.SetShipCoord(MD_AIRCRAFT, 3, coord[3]);
			shipData.SetShipCoord(MD_AIRCRAFT, 4, coord[4]);
			shipData.SetShipCoord(MD_BATTLESHIP, 0, coord[5]);
			shipData.SetShipCoord(MD_BATTLESHIP, 1, coord[6]);
			shipData.SetShipCoord(MD_BATTLESHIP, 2, coord[7]);
			shipData.SetShipCoord(MD_BATTLESHIP, 3, coord[8]);
			shipData.SetShipCoord(MD_CRUISER, 0, coord[9]);
			shipData.SetShipCoord(MD_CRUISER, 1, coord[10]);
			shipData.SetShipCoord(MD_CRUISER, 2, coord[11]);
			shipData.SetShipCoord(MD_DESTROYER1, 0, coord[12]);
			shipData.SetShipCoord(MD_DESTROYER1, 1, coord[13]);
			shipData.SetShipCoord(MD_DESTROYER2, 0, coord[14]);
			shipData.SetShipCoord(MD_DESTROYER2, 1, coord[15]);

	#pragma endregion
		
			shipData.ToMapData(mapData);
			error = network.SubmitMap(mapData);
			if (error == ET_INVALID_MAP)
				puts("��ȿ���� ���� �� �������Դϴ�.");
			while(!over)
			{
				if(DRAW)
					PrintMaps(attackPos);
				error = network.GetPacketType(&type);	//���� ���
				
				switch(type)
				{
				case PKT_SC_ERROR:
					{
						if(error == ET_OPPOSITION_DISCONNECTED)
							puts("������� ������ ���������ϴ�.");
						else
							puts("�� �� ���� �����Դϴ�.");
						}
					break;

				case PKT_SC_MY_TURN:
					{
						attackPos = Attack(m_Atk);
						attackCoord = Coord(attackPos.x, attackPos.y);
						error = network.SubmitAttack(attackCoord);
						printf("%c %c ��ǥ�� ����!!!\n", attackPos.x, attackPos.y);
						if (error == ET_INVALID_ATTACK)
							puts("��ȿ���� ���� ���� ��ġ�Դϴ�.");
					}
					break;

				case PKT_SC_ATTACK_RESULT:
					{
						Network::AttackResultData attackResult = network.GetAttackResult();
						if(attackResult.isMine)
						{
							HitResult hitRes = math.ShortToHitRes(attackResult.attackResult);
							SetHitForNet(attackPos, hitRes);
						}
						else
						{
							HitResult hitRes = math.ShortToHitRes(attackResult.attackResult);
							Position hitPos;
							hitPos.x = attackResult.pos.mX + 'A';
							hitPos.y = attackResult.pos.mY + '1';
							SetHitForNet2(hitPos, hitRes);
						}
						if(DRAW)
							PrintMaps(attackPos);
					}
					break;
				case PKT_SC_GAME_OVER:
					{
						Network::GameResultData gameResult = network.GetGameResult();
						Clear();
						CursorMoveTo(0, 0);
						if(gameResult.isWinner)
							printf("�̰��!\n");
						else
							printf("����!\n");
						printf("%d ���� �ɷȴ�!\n",gameResult.turns);
						if(DRAW)
							Sleep(500);
						over = true;
					}
					break;

				default:
					throw Network::UNEXPECTED_PACKET;
					break;
				}
			}
				
			network.GetPacketType(&type);

			if(type == PKT_SC_NEXT_GAME)
			{
				over = false;
				puts("���� ������ �����ϰ���!\n");
			}
			else if(type == PKT_SC_ALL_OVER)
			{
				Network::FinalResultData finalResult = network.GetFinalResult();
				Clear();
				CursorMoveTo(0, 0);
				printf("%dȸ �̰��!\n",finalResult.winCount);
				printf("��� �� ���� %f\n",finalResult.avgTurns);
				allOver = true;
			}
		}
		network.Disconnect();
	}
	catch(Network::Exception ex)
	{
		switch(ex)
		{
		case Network::NETWORK_ERROR:
			puts("��Ʈ��ũ�� ���� �߻�\n");
			break;
		case Network::SERVER_CLOSED:
			puts("�������� ���� ����\n");
			break;
		case Network::PARAMETER_ERROR:
			puts("�Լ��� ���ڰ� �߸���\n");
			break;
		case Network::UNEXPECTED_PACKET:
			puts("�������� �߸��� ������ ����\n");
			break;
		}
	}
}

Map* Rule::SetupShipsAdv(int playerWho)
{
	Map* ret[PLAYTIMES/2];
	HitResult hitRes = NONE;
	Position attackPos = {0, 0 };
	int count = 0;
	int maxCount = 0;
	int idxCount = 0;
	for (int i = 0; i < PLAYTIMES; ++i) 
	{
		m_TurnedPlayer[playerWho]->PlayerInit();
		m_TurnedPlayer[TurnSwap(playerWho)]->PlayerInit();

		m_MapPlayer[playerWho]->MapInit();
		m_MapPlayer[TurnSwap(playerWho)]->MapInit();
		m_HitMapPlayer[playerWho]->MapInit();
		m_HitMapPlayer[TurnSwap(playerWho)]->MapInit();

		m_TurnedPlayer[TurnSwap(playerWho)]->SetupShips();

		m_MapPlayer[playerWho] = m_TurnedPlayer[playerWho]->GetMap();
		m_MapPlayer[TurnSwap(playerWho)] = m_TurnedPlayer[TurnSwap(playerWho)]->GetMap();

		count = 0;
		ret[idxCount] = m_Atk->GetMap();
		do
		{
			attackPos = Attack(m_TurnedPlayer[playerWho]);
			hitRes = m_TurnedPlayer[TurnSwap(playerWho)]->Defend(attackPos);
			SetHit(attackPos, hitRes, playerWho);
			count += i % 2;	// Ȧ�� �ϸ��� �� ����
		}while(( m_TurnedPlayer[0]->GetHasShip() != 0 ) &&
			( m_TurnedPlayer[1]->GetHasShip() != 0 ));

		if(count > maxCount)
		{
			maxCount = count;
			idxCount++;
			PrintMaps(attackPos);
			printf("�� �� : %d", count);
			getchar();
		}
	}
	
	Math math;
	for(int x = 0; x < MapScale; ++x)
	{
		for(int y = 0; y < MapScale; ++y)
		{
			Position pos;
			pos.x = math.IntToCharX(x);
			pos.y = math.IntToCharY(y);
			int size = ret[idxCount - 1]->ShipSize(x, y);

			switch(size)
			{
			case 5:
				m_Atk->GetShip(0)->AddPosition(pos);
				break;
			case 4:
				m_Atk->GetShip(1)->AddPosition(pos);
				break;
			case 3:
				m_Atk->GetShip(2)->AddPosition(pos);
				break;
			case 2:
				if(m_Atk->GetShip(3)->GetPos(1).x == 0)
					m_Atk->GetShip(3)->AddPosition(pos);
				else
					m_Atk->GetShip(4)->AddPosition(pos);
				break;
			}
		}
	}
	return ret[idxCount - 1];
}

void Rule::SetHit(Position attackPos, HitResult hitRes, int playerWho)
{
	if(attackPos.x == 0 || ( hitRes  < 0 || NONE < hitRes))
		return ;
	// ������ �ʿ� hitRes�� �����ϴ� �Լ��̸� ,
	// �踦 �ı��� ���, ���� �����Ӹ� �ƴ϶� �ı��� ���� ��� ��ǥ�� ���� hitRes�� ���� �ϱ� ���� �Լ� .
	// Rule �� ������ �ִ� ������ ������ �����ϴ� Map �� HitRes �߰�
	bool isHit = false;
	if(( hitRes != MISS ) && ( hitRes != HIT ) && ( hitRes != NONE ))
	{
		for(int i = 0; i < MAX_SHIP_LENGTH; ++i)
		{
			Ship* ship = m_TurnedPlayer[TurnSwap(playerWho)]->GetShip(i);
			for(int s = 0; s < ship->GetSize(); ++s)
			{
				if(( ship->GetPos(s).x == attackPos.x ) && ( ship->GetPos(s).y == attackPos.y ))
				{
					isHit = true;
					break;
				}
			}	// ���ݹ��� �踦 �˾Ƴ��� ���� for ���� .
			if(isHit)	// �� �谡 DESTROY �� �谡 ���� ��� ,
			{
				for(int s = 0; s < ship->GetSize(); ++s)
				{
					m_TurnedPlayer[playerWho]->GetEnomyShip(i)->SetHP(0);
					m_TurnedPlayer[playerWho]->GetEnomyMap()->SetHit(ship->GetPos(s), hitRes);
					m_MapPlayer[playerWho]->SetHit(ship->GetPos(s), hitRes);
					m_HitMapPlayer[TurnSwap(playerWho)]->SetHit(ship->GetPos(s), hitRes);
				}	// �� ���� ������ ��ġ�Ǵ� ��ǥ ���θ� hitRes �� ������ .
				return ;
			}
		}
	}
	// �谡 �ı����� �ʾҴٸ� �׳� �Է�
	m_TurnedPlayer[playerWho]->GetEnomyMap()->SetHit(attackPos, hitRes);
	m_MapPlayer[playerWho]->SetHit(attackPos, hitRes);
	m_HitMapPlayer[TurnSwap(playerWho)]->SetHit(attackPos, hitRes);
}

void Rule::SetHitForNet2(Position attackPos, HitResult hitRes)
{
	if(attackPos.x == 0 || ( hitRes  < 0 || NONE < hitRes))
		return ;
	// ������ �ʿ� hitRes�� �����ϴ� �Լ��̸� ,
	// �踦 �ı��� ���, ���� �����Ӹ� �ƴ϶� �ı��� ���� ��� ��ǥ�� ���� hitRes�� ���� �ϱ� ���� �Լ� .
	// Rule �� ������ �ִ� ������ ������ �����ϴ� Map �� HitRes �߰�
	bool isHit = false;
	if(( hitRes != MISS ) && ( hitRes != HIT ) && ( hitRes != NONE ))
	{
		for(int i = 0; i < MAX_SHIP_LENGTH; ++i)
		{
			Ship* ship = m_Atk->GetShip(i);
			for(int s = 0; s < ship->GetSize(); ++s)
			{
				if(( ship->GetPos(s).x == attackPos.x ) && ( ship->GetPos(s).y == attackPos.y ))
				{
					isHit = true;
					break;
				}
			}	// ���ݹ��� �踦 �˾Ƴ��� ���� for ���� .
			if(isHit)	// �� �谡 DESTROY �� �谡 ���� ��� ,
			{
				for(int s = 0; s < ship->GetSize(); ++s)
				{
					m_MapPlayer[1]->SetHit(ship->GetPos(s), hitRes);
					m_HitMapPlayer[0]->SetHit(ship->GetPos(s), hitRes);
				}	// �� ���� ������ ��ġ�Ǵ� ��ǥ ���θ� hitRes �� ������ .
				return ;
			}
		}
	}
	m_MapPlayer[1]->SetHit(attackPos, hitRes);
	m_HitMapPlayer[0]->SetHit(attackPos, hitRes);
}

void Rule::SetHitForNet(Position attackPos, HitResult hitRes)
{
	if(attackPos.x == 0 || ( hitRes  < 0 || NONE < hitRes))
		return ;

	Math math;
	PositionGroup pg;
	PositionGroup pg2;
	int size;
	int count = 0;
	bool out1 = true;
	bool out2 = true;
	int _x = math.CharToIntX(attackPos.x);
	int _y = math.CharToIntY(attackPos.y);
	

	if( hitRes != MISS && hitRes != NONE && hitRes != HIT)
	{
		switch(hitRes)
		{
		case AIRCRAFT_DESTROY:
			m_Atk->GetEnomyShip(0)->SetHP(0);
			size = 5;
			break;
		case BATTLESHIP_DESTROY:
			m_Atk->GetEnomyShip(1)->SetHP(0);
			size = 4;
			break;
		case CRUISER_DESTROY:
			m_Atk->GetEnomyShip(2)->SetHP(0);
			size = 3;
			break;
		case DESTROYER_DESTROY:
			if(m_Atk->GetEnomyShip(3)->GetHP() != 0)
			{
				m_Atk->GetEnomyShip(3)->SetHP(0);
			}
			else
			{
				m_Atk->GetEnomyShip(4)->SetHP(0);
			}
			size = 2;
			break;
		}
		
		m_Atk->GetEnomyMap()->SetHit(attackPos, hitRes);
		m_MapPlayer[0]->SetHit(attackPos, hitRes);
		m_HitMapPlayer[1]->SetHit(attackPos, hitRes);

		for(int x = 0; x < MapScale; x++)
		{
			for(int y = 0; y < MapScale; y++)
			{
				out1 = true;
				out2 = true;
				for(int s = 0; s < size; s++)
				{
					if(( m_Atk->GetEnomyMap()->IsHit(x + s, y) == HIT || m_Atk->GetEnomyMap()->IsHit(x + s, y) == hitRes ) && x + s < MapScale)
					{
						pg.pos[s].x = math.IntToCharX(x + s);
						pg.pos[s].y = math.IntToCharY(y);
						if(m_Atk->GetEnomyMap()->IsHit(x + s, y) == hitRes)
							out1 = false;
						if(x + s == _x && y == _y)
							out2 = false;
						if(s == size - 1 && !out1 && !out2)
						{
							count++;
							pg2 = pg;
						}
						continue;
					}
					break;
				}
				
				out1 = true;
				out2 = true;
				for(int s = 0; s < size; s++)
				{
					if(( m_Atk->GetEnomyMap()->IsHit(x, y + s) == HIT || m_Atk->GetEnomyMap()->IsHit(x, y + s) == hitRes ) && y + s < MapScale)
					{
						pg.pos[s].x = math.IntToCharX(x);
						pg.pos[s].y = math.IntToCharY(y + s);
						if(m_Atk->GetEnomyMap()->IsHit(x, y + s) == hitRes)
							out1 = false;
						if(x == _x && y + s == _y)
							out2 = false;
						if(s == size - 1 && !out1 && !out2)
						{
							count++;
							pg2 = pg;
						}
						continue;
					}
					break;
				}
			}
		}

		if(count <= 1)
		{
			for(int s = 0; s < size; ++s)
			{
				m_Atk->GetEnomyMap()->SetHit(pg2.pos[s], hitRes);
				m_MapPlayer[0]->SetHit(pg2.pos[s], hitRes);
				m_HitMapPlayer[1]->SetHit(pg2.pos[s], hitRes);
			}

			for(int i = m_IdxForSetNet - 1; i > 0; --i)
			{
				SetHitForNet(m_PosForSetNet[i], m_HitResForSetNet[i]);
			}
		}
		else
		{
			m_Atk->GetEnomyMap()->SetHit(attackPos, hitRes);
			m_MapPlayer[0]->SetHit(attackPos, hitRes);
			m_HitMapPlayer[1]->SetHit(attackPos, hitRes);

			m_PosForSetNet[m_IdxForSetNet] = attackPos;
			m_HitResForSetNet[m_IdxForSetNet] = hitRes;
			m_IdxForSetNet++;
		}
		return ;
	}
	m_Atk->GetEnomyMap()->SetHit(attackPos, hitRes);
	m_MapPlayer[0]->SetHit(attackPos, hitRes);
	m_HitMapPlayer[1]->SetHit(attackPos, hitRes);
}

Position Rule::Attack(Player* pAtk)
{
	Position attackPos = {0, };
	if(pAtk == nullptr)
		return attackPos;

	if(pAtk->GetPlayerType() == nPlayer::NORMAL)
		return pAtk->PlayerAttack();

	if(pAtk->GetPlayerType() == nPlayer::AI)
		return pAtk->AiAttack();

	return attackPos;
}

void Rule::PrintHitResult(HitResult hitRes)
{
	switch(hitRes)
	{
	case NONE:
		printf_s(" MISS :(\n");
		break;
	case MISS:
		printf_s(" MISS :(\n");
		break;
	case HIT:
		printf_s(" HIT! :)\n");
		break;
	case AIRCRAFT_DESTROY:
		printf_s(" AIRCRAFT_DESTROY! :D\n");
		break;
	case BATTLESHIP_DESTROY:
		printf_s(" BATTLESHIP_DESTROY! :D\n");
		break;
	case CRUISER_DESTROY:
		printf_s(" CRUISER_DESTROY! :D\n");
		break;
	case DESTROYER_DESTROY:
		printf_s(" DESTROYER_DESTROY! :D\n");
		break;
	}
}

void Rule::PrintMaps(Position attackPos)
{
	//if(attackPos.x == 0) return ;

	CursorMoveTo(XStartForPlayer[0], YStartForPlayer[0] - 1);
	printf_s(" Player 1\n");
	
	CursorMoveTo(XStartForPlayer[1], YStartForPlayer[1] - 1);
	printf_s(" Player 2\n");

	switch(m_GameType)
	{
	case PVP:
		PrintMap(0);
		PrintMap(1);
		break;

	case PVE:
		PrintShip(attackPos, 0);
		PrintMap(1);
		break;
		//ù��° �÷��̾ �÷��̾�� ������ . ������ ����ų �� �ִ� �κ�

	case EVE:
		PrintShip(attackPos, 0);
		PrintShip(attackPos, 1);
		break;

	case EVN:
		PrintShip(attackPos, 0);
		PrintShip(attackPos, 1);
		break;
	}
}

void Rule::PrintMap(int playerWho)
{
	//�����ڰ� ������ ������(IsHit) �� ��ϵ�, �谡 �Ⱥ��̴� ���� �����ִ� �Լ�
	if(playerWho != 0 && playerWho != 1)
		return ;

	Math math;
	int xs = XStartForPlayer[playerWho];
	int ys = YStartForPlayer[playerWho];
	
	CursorMoveTo(xs, ys);
	
	math.SetColor(PONT_COLOR, BASE_COLOR);
	for(int x = 0; x < MapScale; x++)
	{
		printf_s("   %c", x + 'A');
	}
	CursorMoveTo(xs, ++ys);

	for(int y = 0; y < MapScale; y++)
	{
		CursorMoveTo(xs, ys + y * 2);

		math.SetColor(PONT_COLOR, BASE_COLOR);
		printf_s("%c ", y + '1');
		for(int x = 0; x < MapScale; x++)
		{
			CursorMoveTo(xs + x * 4 + 2, ys + y * 2);
			DrawHitResChar(NULL, m_HitMapPlayer[playerWho]->IsHit(x, y), NOTSEEN);

			math.SetColor(BASE_COLOR, BASE_COLOR);
			printf_s("  ");
		}
		CursorMoveTo(0, 0);
		//Ŀ���� �����ϴ� �Լ��� ���� �� 0, 0���� �ʱ�ȭ�� ��
	}
	math.SetColor(PONT_COLOR, BASE_COLOR);
	//���� �����ϴ� �Լ��� ���� �� PONT_COLOR, BASE_COLOR �� �ʱ�ȭ �� ��
}

void Rule::PrintShip(Position attackPos, int playerWho)
{
	//if(attackPos.x == 0 || ( playerWho != 0 && playerWho != 1)) return ;

	Math math;
	int xs = XStartForPlayer[playerWho];
	int ys = YStartForPlayer[playerWho];

	CursorMoveTo(xs, ys);

	math.SetColor(PONT_COLOR, BASE_COLOR);
	for(int i = 0; i < MapScale; i++)
	{
		printf_s("   %c", i + 'A');
	}
	CursorMoveTo(xs, ++ys);

	for(int y = 0; y < MapScale; y++)
	{
		CursorMoveTo(xs, ys + y * 2);

		math.SetColor(PONT_COLOR, BASE_COLOR);
		printf_s("%c ", y + '1');

		for(int x = 0; x < MapScale; x++)
		{
			CursorMoveTo(xs + x * 4 + 2, ys + y * 2);

			int size = m_MapPlayer[playerWho]->ShipSize(x, y);
			DrawHitResChar(size, m_HitMapPlayer[playerWho]->IsHit(x, y), SEEN);
			
			/*
			if(ship == nullptr)
				PrintString(math.CharToIntX(attackPos.x), math.CharToIntY(attackPos.y),
					"��", YELLOW, RED);
			//��� ������ ��ǥ�� ���̶���Ʈ
			*/

			math.SetColor(BASE_COLOR, BASE_COLOR);
			printf_s("  ");
		}
		CursorMoveTo(BASE_COLOR, BASE_COLOR);
	}
	math.SetColor(PONT_COLOR, BASE_COLOR);
}

void Rule::DrawHitResChar(int shipSize, int hitRes, int drawType)
{
	if(( shipSize < 0 || MAX_SHIP_LENGTH < shipSize) || ( hitRes < 0 || NONE < hitRes ) || ( drawType != SEEN && drawType != NOTSEEN))
		return ;

	Math math;
		
	if(drawType == SEEN)
	{
		//math.SetColor(RED, BLACK);	//���� ũ�⸦ �� �� ����
		//printf("%d", shipSize);
		switch(shipSize)
		{
		case 0:
			if(hitRes == MISS)
			{
				//�谡 ���� ����
				math.SetColor(RED, BLACK);
				printf_s("��");
			}
			else
			{
				//�谡 ���� �ȸ���
				math.SetColor(BACK_COLOR, BACK_COLOR);
				printf_s("  ");
			}
			break;
		case 2:
			if(hitRes == HIT)
			{
				math.SetColor(DARKYELLOW, RED);
				printf_s("��");
			}
			else if(hitRes == NONE || hitRes == MISS)
			{
				math.SetColor(YELLOW, BACK_COLOR);
				printf_s("��");
			}
			else if(hitRes == DESTROYER_DESTROY)
			{
				math.SetColor(DARKYELLOW, BLACK);
				printf_s("��");
			}
			break;
		case 3:
			if(hitRes == HIT)
			{
				math.SetColor(DARKGREEN, RED);
				printf_s("��");
			}
			else if(hitRes == NONE || hitRes == MISS)
			{
				math.SetColor(GREEN, BACK_COLOR);
				printf_s("��");
			}
			else if(hitRes == CRUISER_DESTROY)
			{
				math.SetColor(DARKGREEN, BLACK);
				printf_s("��");
			}
			break;
		case 4:
			if(hitRes == HIT)
			{
				math.SetColor(DARKBLUE, RED);
				printf_s("��");
			}
			else if(hitRes == NONE || hitRes == MISS)
			{
				math.SetColor(BLUE, BACK_COLOR);
				printf_s("��");
			}
			else if(hitRes == BATTLESHIP_DESTROY)
			{
				math.SetColor(DARKBLUE, BLACK);
				printf_s("��");
			}
			break;
		case 5:
			if(hitRes == HIT)
			{
				math.SetColor(DARKRED, RED);
				printf_s("��");
			}
			else if(hitRes == NONE || hitRes == MISS)
			{
				math.SetColor(RED, BACK_COLOR);
				printf_s("��");
			}
			else if(hitRes == AIRCRAFT_DESTROY)
			{
				math.SetColor(DARKRED, BLACK);
				printf_s("��");
			}
			break;
		}
	}
	else if(drawType == NOTSEEN)
	{
		switch (hitRes)
		{
		case NONE:
			math.SetColor(BACK_COLOR, BACK_COLOR);
			printf_s("  ");
			break;
		case MISS:
			math.SetColor(RED, BLACK);
			printf_s("��");
			break;
		case HIT:
			math.SetColor(RED, BACK_COLOR);
			printf_s("��");
			break;
		case DESTROYER_DESTROY:
			math.SetColor(DARKYELLOW, BLACK);
			printf_s("��");
			break;
		case CRUISER_DESTROY:
			math.SetColor(DARKGREEN, BLACK);
			printf_s("��");
			break;
		case BATTLESHIP_DESTROY:
			math.SetColor(DARKBLUE, BLACK);
			printf_s("��");
			break;
		case AIRCRAFT_DESTROY:
			math.SetColor(DARKRED, BLACK);
			printf_s("��");
			break;
		}
	}
}

Ship* Rule::GetShipOfPosition(int playerWho, int x, int y)	//Ư�� ��ǥ�� �谡 �ִ��� Ȯ���ϰ� ��ȯ���� .
{
	if(( playerWho != 0 && playerWho != 1) || ( x < 0 || MapScale < x ) || ( y < 0 || MapScale < y ))
		return nullptr;

	Position pos;
	Math math;
	pos.x = math.IntToCharX(x);
	pos.y = math.IntToCharY(y);

	for(int i = 0; i < MAX_HAVE_SHIPS; i++)
	{
		Ship* ship = m_TurnedPlayer[playerWho]->GetShip(i);
		for(int s = 0; s < ship->GetSize(); s++)
		{
			if(( ship->GetPos(s).x == pos.x )&&( ship->GetPos(s).y == pos.y ))
				return ship;
		}
	}
	return nullptr;
}

void Rule::CursorMoveTo(int x, int y)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition( hConsole, pos );
}

void Rule::InitializeScreen()
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // �ڵ��� ���ϰ�

    ConsoleCursor.bVisible = false; // true ���� , false �Ⱥ���
    ConsoleCursor.dwSize = 1; // Ŀ�� ������

    SetConsoleCursorInfo(hConsole , &ConsoleCursor); // ����
}

void Rule::Clear()
{
	Math math;
	math.SetColor(PONT_COLOR, BASE_COLOR);
	CursorMoveTo(0, 0);
	printf("                                                        \n");
	printf("                                                        \n");
	CursorMoveTo(0, 0);
}
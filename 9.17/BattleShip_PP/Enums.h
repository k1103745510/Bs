#pragma once
#ifdef _DEBUG
#pragma comment(lib, "BGNL_debug.lib")
#else
#pragma comment(lib, "BGNL.lib")
#endif
#include "Network.h"

#define MAX_SHIP_LENGTH			5
#define AIRCRAFT_LENGTH 5
#define BATTLESHIP_LENGTH 4
#define CRUISER_LENGTH 3
#define DESTROYER_LENGTH 2

struct Position
{
	char x;
	char y;
};

struct PositionGroup
{
	Position pos[MAX_SHIP_LENGTH];
};

struct MapData
{
	Coord aircraft[AIRCRAFT_LENGTH];
	Coord battleship[BATTLESHIP_LENGTH];
	Coord cruiser[CRUISER_LENGTH];
	Coord destroyer1[DESTROYER_LENGTH];
	Coord destroyer2[DESTROYER_LENGTH];
};

namespace nPlayer
{
	enum PlayerType
	{
		NORMAL,
		AI,
		NETWORK,
		NONE,
	};
}

enum HitResult
{
	MISS,
	HIT,
	DESTROYER_DESTROY,
	CRUISER_DESTROY,
	BATTLESHIP_DESTROY,
	AIRCRAFT_DESTROY,
	NONE,
};

enum DrawType
{
	SEEN,
	NOTSEEN,
};

enum GameType
{
	PVP,
	PVE,
	EVE,
	EVN,
};

namespace nShip
{
	enum Type
	{
		AIRCRAFT,
		CRUISER,
		DESTROYER,
		BATTLESHIP,
		NONE,
	};
}

class Enums
{
public:
	Enums(void);
	~Enums(void);
};


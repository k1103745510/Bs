#pragma once

#include "Ship.h"

class Destroyer :
	public Ship
{
public:
	Destroyer();
	~Destroyer();
	
	HitResult HitCheck(Position pos);
};


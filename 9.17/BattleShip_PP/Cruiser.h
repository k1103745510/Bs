#pragma once

#include "Ship.h"

class Cruiser :
	public Ship
{
public:
	Cruiser();
	~Cruiser();
	
	HitResult HitCheck(Position pos);
};

#include "stdafx.h"
#include "Math.h"


Math::Math(void)
{
}


Math::~Math(void)
{
}


void Math::SetColor(int color , int bgcolor)
{
	bgcolor &= 0xf ;
	color &= 0xf ;
	SetConsoleTextAttribute(GetStdHandle( STD_OUTPUT_HANDLE ) , color | ( bgcolor << 4 ) ) ;  
}

HitResult Math::ShortToHitRes(short i)
{
	switch (i)
	{
	case AR_NONE:
		return NONE;
	case AR_MISS:
		return MISS;
	case AR_HIT:
		return HIT;
	case AR_DESTROY_AIRCRAFT:
		return AIRCRAFT_DESTROY;
	case AR_DESTROY_BATTLESHIP:
		return BATTLESHIP_DESTROY;
	case AR_DESTROY_CRUISER:
		return CRUISER_DESTROY;
	case AR_DESTROY_DESTROYER:
		return DESTROYER_DESTROY;
	}
	return NONE;
}
// BattleShip_PP.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include <Windows.h>
#include "stdafx.h"
#pragma comment(lib, "winmm.lib")


int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	srand((unsigned)time(NULL));

	system("mode con: lines=30 cols=95");

	Math math;
	math.SetColor(PONT_COLOR, 0);
	
	Rule rule;
	rule.Start();

	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	system("pause");
	return 0;
}


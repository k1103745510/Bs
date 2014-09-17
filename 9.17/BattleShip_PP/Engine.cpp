#include "stdafx.h"
#include "Engine.h"
#include "Rule.h"


Engine::Engine(void)
{
	m_Rule = new Rule();
}


Engine::~Engine(void)
{
}

void Engine::GameStart()
{
	GetRule()->Start();
}
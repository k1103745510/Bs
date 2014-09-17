#pragma once

#include "stdafx.h"
#include "Enums.h"
#include "Rule.h"

class Engine
{
public:
	Engine(void);
	~Engine(void);

	void GameStart();
	Rule* GetRule(){ return m_Rule; }

protected:
	Rule* m_Rule;
};


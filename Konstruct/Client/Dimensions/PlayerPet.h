#pragma once
#include "NPC.h"

class AIControl;
class PlayerCharacter;
class Enemy;

class PlayerPet :
	public NPC
{
public:
	PlayerPet(PlayerCharacter* pOwner, int iLevel);
	PlayerPet(Enemy* pConvert, PlayerCharacter* pOwner);
	~PlayerPet(void);

protected:
	AIControl*			m_pAIBehavior;
	PlayerCharacter*	m_pOwner;
};

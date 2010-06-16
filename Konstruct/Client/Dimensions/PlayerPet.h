#pragma once
#include "NPC.h"

class AIControl;
class PlayerCharacter;

class PlayerPet :
	public NPC
{
public:
	PlayerPet(PlayerCharacter* pOwner, int iLevel);
	~PlayerPet(void);

protected:
	AIControl*			m_pAIBehavior;
	PlayerCharacter*	m_pOwner;
};

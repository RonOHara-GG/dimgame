#include "StdAfx.h"
#include "PlayerPet.h"
#include "PlayerPetAI.h"

PlayerPet::PlayerPet(PlayerCharacter* pOwner, int iLevel)
{
	m_pOwner = pOwner;
	m_iLevel = iLevel;
	m_pAIBehavior = new PlayerPetAI(this);
}

PlayerPet::~PlayerPet(void)
{
}

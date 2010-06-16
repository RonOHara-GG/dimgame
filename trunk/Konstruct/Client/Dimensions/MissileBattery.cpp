#include "StdAfx.h"
#include "MissileBattery.h"
#include "PlayerCharacter.h"

MissileBattery::MissileBattery(PlayerCharacter* pOwner, int iLevel, BatteryType eType):
	PlayerPet(pOwner, iLevel)
{	
	m_eBatteryType = eType;
	m_bDead = false;
}

MissileBattery::~MissileBattery(void)
{
}

bool MissileBattery::Update(float fGameTime)
{
	return !m_bDead;
}

void MissileBattery::Unequip(PlayerCharacter* pPlayer)
{
	pPlayer->RemovePet(this);
	m_bDead = true;
}

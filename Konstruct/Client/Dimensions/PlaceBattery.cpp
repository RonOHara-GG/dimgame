#include "StdAfx.h"
#include "PlaceBattery.h"
#include "PlayerCharacter.h"

PlaceBattery::PlaceBattery(void)
{
}

PlaceBattery::~PlaceBattery(void)
{
}

bool PlaceBattery::Activate(PlayerCharacter* pSkillOwner)
{
	if( m_bReady )
	{
		m_fElaspedSinceCast = 0.0f;

		//Get the target tile that it will be placed
		m_iTargetTile = GetTargetTile();

		//make sure tile is in range
		if( !InRange(pSkillOwner, m_iTargetTile, GetRange()) )
			return false;
		
		pSkillOwner->SetActiveSkill(this);
		return true;
	}
	return false;
}

bool PlaceBattery::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	//place battery according to animation
	if( true )
	{
		//place battery			
		pSkillOwner->EquipSecondary(new MissileBattery(pSkillOwner, 1, m_eBatteryType));		

		return false;

	}
	

	return true;
}

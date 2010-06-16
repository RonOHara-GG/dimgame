#include "StdAfx.h"
#include "PlaceBattery.h"
#include "PlayerCharacter.h"

#define MAX_SPEED 50

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
		m_fSpeed = MAX_SPEED - (m_fSpeedMod * m_iSkillRank);
		m_fElaspedSinceCast = 0.0f;
		m_iTargetTile = -1;
		pSkillOwner->SetActiveSkill(this);
		return true;
	}
	return false;
}

bool PlaceBattery::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	//get location to place battery

	if( m_iTargetTile > -1 )
	{

		m_fElaspedSinceCast += fDeltaTime;

		if( m_fElaspedSinceCast >= m_fSpeed )
		{
			//place battery			
			pSkillOwner->EquipSecondary(new MissileBattery(pSkillOwner, 1, m_eBatteryType));		

			return false;

		}
	}

	return true;
}

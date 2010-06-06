#include "StdAfx.h"
#include "RapidShot.h"
#include "Weapon.h"
#include "PlayerCharacter.h"
#include "Grid.h"
#include "Level.h"

#define MIN_SHOT_COUNT 4

RapidShot::RapidShot(void)
{
}

RapidShot::~RapidShot(void)
{
}

bool RapidShot::Activate(PlayerCharacter *pSkillOwner)
{
	if( m_bReady )
	{
		m_fDamage = pSkillOwner->GetEquippedWeapon()->GetDamage() * (m_fDamageMultiple + (m_fRankMultipleMax * m_iSkillRank));
		m_fRange = pSkillOwner->GetEquippedWeapon()->GetRange() * m_fRangeMultiple;
		m_iShotCount = MIN_SHOT_COUNT + (m_iSkillRank * m_fShotMultiple);
		m_fSpeed = pSkillOwner->GetEquippedWeapon()->GetSpeed() * m_fSpeedMultiple;

		m_bReady = false;
		m_bExecuted = false;
		m_fDistTraveled = 0.0f;
		m_vLocation = pSkillOwner->GetLocation();
		m_pLastHit = 0;

		return true;
	}

	return false;
}

bool RapidShot::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	return false;
}

#include "StdAfx.h"
#include "PatchWounds.h"
#include "PlayerCharacter.h"
#include "HealOverTime.h"

#define MIN_DURATION 2
#define MAX_SPEED 3

PatchWounds::PatchWounds(void)
{
}

PatchWounds::~PatchWounds(void)
{
}

bool PatchWounds::Activate(PlayerCharacter *pSkillOwner)
{
	if( m_bReady )
	{
		int iHeal = m_iHealImdMin + ( rand() % (m_iHealImdMax - m_iHealImdMin + 1) );
		m_iHealImd = iHeal * m_iSkillRank;

		iHeal = m_iHealPSMin + ( rand() % (m_iHealPSMax - m_iHealPSMin + 1) );
		m_iHealPS = iHeal * m_iSkillRank;

		m_fDuration = MIN_DURATION + ( m_iSkillRank * m_fDurationModifier );

		m_fSpeed = MAX_SPEED - (m_iSkillRank * m_fSpeedModifier);

		if( pSkillOwner->GetTarget() )
		{
			m_pTarget = pSkillOwner->GetTarget();
		}
		else
			m_pTarget = pSkillOwner;

		m_bReady = false;
		m_bExecuted = false;
		m_fElaspedSinceCast = 0.0f;
		pSkillOwner->SetActiveSkill(this);

		

		return true;
	}

	return false;

}

bool PatchWounds::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if( m_fElaspedSinceCast >= m_fSpeed )
	{
		m_pTarget->Heal(m_iHealImd);
		m_pTarget->AddPersistentSkill(new HealOverTime(m_iHealPS, m_fDuration));
		return false;
	}	
	
	return true;
}

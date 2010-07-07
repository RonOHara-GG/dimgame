#include "StdAfx.h"
#include "PatchWounds.h"
#include "PlayerCharacter.h"
#include "HealOverTime.h"

PatchWounds::PatchWounds(void)
{
}

PatchWounds::~PatchWounds(void)
{
}

bool PatchWounds::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	if( true )
	{
		int iHeal = m_iHealImdMin + ( rand() % (m_iHealImdMax - m_iHealImdMin + 1) );
		int iHealImd = iHeal * m_iSkillRank;

		iHeal = m_iHealPSMin + ( rand() % (m_iHealPSMax - m_iHealPSMin + 1) );
		int iHealPS = iHeal * m_iSkillRank;

		float fDuration = m_fMinDuration + ( m_iSkillRank * m_fDurationModifier );

		m_pTarget->Heal(iHealImd);
		m_pTarget->AddPersistentSkill(new HealOverTime(iHealPS, fDuration));
		return false;
	}	
	
	return true;
}

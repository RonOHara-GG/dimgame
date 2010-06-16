#include "StdAfx.h"
#include "Resurrect.h"
#include "PlayerCharacter.h"

#define MIN_HEAL 10
#define MIN_RANGE 4

Resurrect::Resurrect(void)
{
}

Resurrect::~Resurrect(void)
{
}

bool Resurrect::Activate(PlayerCharacter *pSkillOwner)
{
	if( m_bReady )
	{
		m_iRange = MIN_RANGE + (m_iSkillRank / m_iRangeMod);
		m_fHeal = MIN_HEAL + (m_iSkillRank * m_fHealMod);
		
		m_fElaspedSinceCast = 0.0f;
		m_bReady = false;
		m_bExecuted = false;
		m_pTarget = 0;

		pSkillOwner->SetActiveSkill(this);
		return true;
	}

	return false;
}

bool Resurrect::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	//get target

	if( !m_pTarget )
	{
		if( m_pTarget->IsAlive() )
		{
			//cancel skill
			return true;
		}

		m_fElaspedSinceCast += fDeltaTime;

		if( m_fElaspedSinceCast >= m_fSpeed )
		{
			m_pTarget->Heal(m_pTarget->GetMaxHealth() * m_fHeal);
			m_pTarget->HealMental(m_pTarget->GetMaxMental() * m_fHeal);

			m_bExecuted = true;
			return true;

		}
	}

	return false;
}

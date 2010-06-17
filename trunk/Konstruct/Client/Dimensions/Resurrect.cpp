#include "StdAfx.h"
#include "Resurrect.h"
#include "PlayerCharacter.h"

#define MIN_HEAL 10

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
		m_iRange = m_iMinRange + (m_iSkillRank / m_iRangeMod);
		m_iHeal = MIN_HEAL + (m_iSkillRank * m_iHealMod);
		
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
			m_pTarget->Heal(m_pTarget->GetMaxHealth() * m_iHeal);
			m_pTarget->HealMental(m_pTarget->GetMaxMental() * m_iHeal);

			m_bExecuted = true;
			return true;

		}
	}

	return false;
}

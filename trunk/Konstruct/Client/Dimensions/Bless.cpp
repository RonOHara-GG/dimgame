#include "StdAfx.h"
#include "Bless.h"
#include "BlessBuff.h"
#include "PlayerCharacter.h"

#define MIN_DURATION 60

Bless::Bless(void)
{
}

Bless::~Bless(void)
{
}

bool Bless::Activate(PlayerCharacter *pSkillOwner)
{
	if( m_bReady )
	{
		int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );
		m_fDamageBonus = iRankMultiple * m_iSkillRank;
		
		m_fDuration = MIN_DURATION + (m_fDurationMod * m_iSkillRank);

		iRankMultiple = m_iDamageReductionMin + ( rand() % (int)(m_iDamageReductionMax - m_iDamageReductionMin) );
		m_fDamageReduction = iRankMultiple * m_iSkillRank;

		m_iRange = m_iMinRange + (m_iSkillRank / m_iRangeMod);

		m_fElaspedSinceCast = 0.0f;
		m_bReady = false;
		m_bExecuted = false;
		m_pTarget = 0;

		pSkillOwner->SetActiveSkill(this);

		return true;
	}
	
	return false;
}

bool Bless::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	//Get target

	if( m_pTarget )
	{
		m_fElaspedSinceCast += fDeltaTime;

		if( m_fElaspedSinceCast >= m_fSpeed )
		{			
			m_pTarget->AddPersistentSkill(new BlessBuff(m_pTarget, m_fDamageBonus, m_fDamageReduction, m_fDuration));
			m_bExecuted = true;

			return true;
		}
	}
	return false;
}
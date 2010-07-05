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

bool Bless::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	if( m_pTarget )
	{
		//cast with animation
		if( true )
		{			
			int iRankMultiple = m_iRankMultipleMin + ( rand() % (m_iRankMultipleMax - m_iRankMultipleMin) );
			int iDamageBonus = iRankMultiple * m_iSkillRank;

			iRankMultiple = m_iDamageReductionMin + ( rand() % (m_iDamageReductionMax - m_iDamageReductionMin) );
			int iDamageReduction = iRankMultiple * m_iSkillRank;			
		
			float fDuration = MIN_DURATION + (m_fDurationMod * m_iSkillRank);

			m_pTarget->AddPersistentSkill(new BlessBuff(m_pTarget, iDamageBonus, iDamageReduction, fDuration));
			m_bExecuted = true;

			return false;
		}
	}
	return true;
}
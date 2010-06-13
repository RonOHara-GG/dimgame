#include "StdAfx.h"
#include "HealthyLifestyle.h"
#include "PlayerCharacter.h"

HealthyLifestyle::HealthyLifestyle(PlayerCharacter* pSource):
	SpreadableBuff(pSource)
{
	m_fElaspedSinceCast = 0.0f;
}

HealthyLifestyle::~HealthyLifestyle(void)
{
}

void HealthyLifestyle::Tick(PlayerCharacter* pTarget)
{
	int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );

	pTarget->Heal(iRankMultiple * m_iSkillRank);

}

SpreadableBuff* HealthyLifestyle::CopyBuff()
{
	return new HealthyLifestyle(m_pSource);
}
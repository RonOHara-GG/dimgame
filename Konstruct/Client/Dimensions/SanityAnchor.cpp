#include "StdAfx.h"
#include "SanityAnchor.h"
#include "PlayerCharacter.h"

SanityAnchor::SanityAnchor(PlayerCharacter* pSource):
	SpreadableBuff(pSource)
{
}

SanityAnchor::~SanityAnchor(void)
{
}

void SanityAnchor::Tick(PlayerCharacter* pTarget)
{
	int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );

	pTarget->HealMental(iRankMultiple * m_iSkillRank);

}

SpreadableBuff* SanityAnchor::CopyBuff()
{
	return new SanityAnchor(m_pSource);
}

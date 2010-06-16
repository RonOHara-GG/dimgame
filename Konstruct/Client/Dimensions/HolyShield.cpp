#include "StdAfx.h"
#include "HolyShield.h"

#define MIN_RADIUS 3

HolyShield::HolyShield(PlayerCharacter* pSource):
	SpreadableBuff(pSource)
{
}

HolyShield::~HolyShield(void)
{
}

bool HolyShield::Activate(PlayerCharacter *pSkillOwner)
{
	m_fRadius = MIN_RADIUS + (m_iSkillRank / m_fRadiusMod);

	//********************************************************
	//This doesn't make sense having a random damage reduction
	//********************************************************
	int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );

	//pTarget->DamageReduction(iRankMultiple * m_iSkillRank);
	return true;
}
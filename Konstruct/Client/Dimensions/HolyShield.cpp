#include "StdAfx.h"
#include "HolyShield.h"
#include "PlayerCharacter.h"


HolyShield::HolyShield(PlayerCharacter* pSource):
	SpreadableBuff(pSource)
{
	
}

HolyShield::~HolyShield(void)
{
	m_pSource->DamageReduction(-m_iDamageReduction);

}

bool HolyShield::Activate(PlayerCharacter *pSkillOwner)
{
	m_fRadius = m_fMinRadius + (m_iSkillRank / m_fRadiusMod);

	//********************************************************
	//This doesn't make sense having a random damage reduction
	//********************************************************
	int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );
	m_iDamageReduction = iRankMultiple * m_iSkillRank;

	pSkillOwner->DamageReduction(m_iDamageReduction);
	return true;
}

SpreadableBuff* HolyShield::CopyBuff(Actor* pSpreadee)
{
	return new HolyShield(m_pSource);
}
#include "StdAfx.h"
#include "TargetAnalysis.h"
#include "PlayerCharacter.h"

TargetAnalysis::TargetAnalysis(PlayerCharacter* pSource):
	SpreadableBuff(pSource)
{
}

TargetAnalysis::~TargetAnalysis(void)
{
	
}

bool TargetAnalysis::Activate(PlayerCharacter* pSkillOwner)
{
	int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );
	m_iDamageBuff = iRankMultiple * m_iSkillRank;

	pSkillOwner->DamageBuff(m_iDamageBuff);
	m_pTarget = pSkillOwner;

	return true;
}

void TargetAnalysis::Deactivate()
{
	m_pTarget->DamageBuff(-m_iDamageBuff);
}

SpreadableBuff* TargetAnalysis::CopyBuff()
{
	return new TargetAnalysis(m_pSource);
}


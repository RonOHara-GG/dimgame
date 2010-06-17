#include "StdAfx.h"
#include "WeaponSkill.h"

WeaponSkill::WeaponSkill(void)
{
}

WeaponSkill::~WeaponSkill(void)
{
}

bool WeaponSkill::Activate(PlayerCharacter *pSkillOwner)
{
	int iRankMultiple = m_iRankMultipleMin + ( rand() % (m_iRankMultipleMax - m_iRankMultipleMin) );
	m_iDamageBonus = iRankMultiple * m_iSkillRank;
	m_fRangeBonus = m_iSkillRank / m_fRangeMod;
	m_fRecoveryBonus = m_fRecoveryMod * m_iSkillRank;
	m_fSpeedBonus = m_fSpeedMod * m_iSkillRank;

	return true;
}

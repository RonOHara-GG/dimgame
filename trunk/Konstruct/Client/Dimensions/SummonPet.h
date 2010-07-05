#pragma once
#include "skill.h"

class SummonPet :
	public Skill
{
public:
	SummonPet(void);
	~SummonPet(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int	GetRange() { return m_iMinRange + (m_iSkillRank / m_iRangeMod); }

	int		m_iRangeMod;
	int		m_iMinRange;
};

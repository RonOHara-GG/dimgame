#pragma once
#include "skill.h"

class SummonPet :
	public Skill
{
public:
	SummonPet(void);
	~SummonPet(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int		m_iRange;
	int		m_iRangeMod;
};

#pragma once
#include "skill.h"

class Bless :
	public Skill
{
public:
	Bless(void);
	~Bless(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int	GetRange(Actor* p1) { return  m_iMinRange + (m_iSkillRank / m_iRangeMod); }

	int					m_iRangeMod;
	int					m_iMinRange;
	int					m_iDamageReductionMin;
	int					m_iDamageReductionMax;
	float				m_fDurationMod;
	
};

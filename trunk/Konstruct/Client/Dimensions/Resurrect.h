#pragma once
#include "skill.h"

class Resurrect :
	public Skill
{
public:
	Resurrect(void);
	~Resurrect(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);
protected:
	int				GetRange(Actor* p1) { return m_iMinRange + (m_iSkillRank / m_iRangeMod); }

	int				m_iRangeMod;
	int				m_iMinRange;
	int				m_iHealMod;	
};

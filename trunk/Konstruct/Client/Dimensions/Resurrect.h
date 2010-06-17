#pragma once
#include "skill.h"

class Resurrect :
	public Skill
{
public:
	Resurrect(void);
	~Resurrect(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);
protected:
	int				m_iRange;
	int				m_iRangeMod;
	int				m_iMinRange;
	int				m_iHeal;
	int				m_iHealMod;

	PlayerCharacter* m_pTarget;
	
};

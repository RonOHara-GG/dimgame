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
	float			m_fHeal;
	float			m_fHealMod;

	PlayerCharacter* m_pTarget;
	
};

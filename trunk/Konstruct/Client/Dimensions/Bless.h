#pragma once
#include "skill.h"

class Bless :
	public Skill
{
public:
	Bless(void);
	~Bless(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	PlayerCharacter*	m_pTarget;

	int					m_iRange;
	int					m_iRangeMod;
	int					m_iMinRange;
	int					m_iDamageBonus;
	int					m_iDamageReduction;
	int					m_iDamageReductionMin;
	int					m_iDamageReductionMax;
	float				m_fDuration;
	float				m_fDurationMod;
	
};

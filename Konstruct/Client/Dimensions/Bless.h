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
	float				m_fDamageBonus;
	float				m_fDamageReduction;
	int					m_iDamageReductionMin;
	int					m_iDamageReductionMax;
	float				m_fDuration;
	float				m_fDurationMod;
	
};

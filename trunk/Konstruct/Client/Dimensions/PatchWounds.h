#pragma once
#include "skill.h"

class PatchWounds :
	public Skill
{
public:
	PatchWounds(void);
	~PatchWounds(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	float		GetSpeed() { return m_fSpeed - (m_iSkillRank * m_fSpeedModifier); }

	float		m_fMinDuration;
	float		m_fDurationModifier;
	float		m_fSpeedModifier;

	int			m_iHealImdMin;
	int			m_iHealImdMax;
	int			m_iHealPSMin;
	int			m_iHealPSMax;
};

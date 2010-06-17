#pragma once
#include "skill.h"

class PatchWounds :
	public Skill
{
public:
	PatchWounds(void);
	~PatchWounds(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int			m_iHealImd;
	int			m_iHealPS;
	float		m_fDuration;
	float		m_fDurationModifier;
	float		m_fSpeedModifier;

	int			m_iHealImdMin;
	int			m_iHealImdMax;
	int			m_iHealPSMin;
	int			m_iHealPSMax;
	

	Actor* m_pTarget;
};

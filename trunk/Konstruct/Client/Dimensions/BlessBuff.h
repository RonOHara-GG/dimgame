#pragma once
#include "persistentskill.h"

class BlessBuff :
	public PersistentSkill
{
public:
	BlessBuff(Actor* pTarget, float fDamage, float fReduction, float fDuration);
	~BlessBuff(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	float	m_fDamageBonus;
	float	m_fDamageReduction;
};

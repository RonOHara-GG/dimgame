#pragma once
#include "persistentskill.h"

class BlessBuff :
	public PersistentSkill
{
public:
	BlessBuff(Actor* pTarget, int iDamage, int iReduction, float fDuration);
	~BlessBuff(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int		m_iDamageBonus;
	int		m_iDamageReduction;
};

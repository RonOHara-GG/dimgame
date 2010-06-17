#pragma once
#include "persistentskill.h"

class HealOverTime :
	public PersistentSkill
{
public:
	HealOverTime(int iHeal, float fDuration);
	~HealOverTime(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int		m_iHeal;
	float	m_fElapsedTick;
	float	m_fDuration;
	float	m_fElapsedTotal;
};

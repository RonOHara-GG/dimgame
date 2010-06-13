#pragma once
#include "BasicShot.h"
#include "PersistentSkill.h"

class CertainDeath:
	public BasicShot, public PersistentSkill
{
public:
	CertainDeath(void);
	~CertainDeath(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int		m_iExtraTargets;
};

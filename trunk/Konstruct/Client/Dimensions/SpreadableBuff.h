#pragma once
#include "persistentskill.h"

class SpreadableBuff :
	public PersistentSkill
{
public:
	SpreadableBuff(PlayerCharacter* pSource);
	virtual ~SpreadableBuff(void);

	virtual void			Tick(PlayerCharacter* pTarget) { }
	virtual SpreadableBuff*	CopyBuff() { return 0; }

	bool Activate(PlayerCharacter* pSkillOwner) { return true; }
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);	

protected:
	float				m_fRadius;
	PlayerCharacter*	m_pSource;
};

#pragma once
#include "persistentskill.h"

class SpreadableBuff :
	public PersistentSkill
{
public:
	SpreadableBuff(PlayerCharacter* pSource);
	SpreadableBuff(void);
	virtual ~SpreadableBuff(void);

	virtual void			Tick(PlayerCharacter* pTarget) { }
	virtual SpreadableBuff*	CopyBuff() { return 0; }

	bool Activate(PlayerCharacter* pSkillOwner) { return true; }
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);	

protected:
	virtual SpreadableBuff*    CopyBuff(Actor* pSpreadee) { return new SpreadableBuff(m_pSource); }

	float				m_fRadius;
	PlayerCharacter*	m_pSource;
	Actor*				m_pInfected;
};

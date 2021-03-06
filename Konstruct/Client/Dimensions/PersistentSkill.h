#pragma once
#include "skill.h"

class PersistentSkill :
	public Skill
{
public:
	PersistentSkill(void);
	PersistentSkill(Actor* pTarget);
	virtual ~PersistentSkill(void);

	virtual void Deactivate() { };

protected:
	Actor*	m_pTarget;

};

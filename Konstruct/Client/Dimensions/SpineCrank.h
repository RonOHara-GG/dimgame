#pragma once
#include "bearhug.h"

class SpineCrank :
	public BearHug
{
public:
	SpineCrank(void);
	~SpineCrank(void);

	bool Activate(PlayerCharacter* pSkillOwner);

protected:
	float m_fBaseSpeed;
	Actor* m_pTarget;
};

#pragma once
#include "basicshot.h"

class BounceShot :
	public BasicShot
{
public:
	BounceShot(void);
	~BounceShot(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	float	m_fBounceModifier;

};



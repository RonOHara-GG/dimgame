#pragma once
#include "basicshot.h"

class BounceShot :
	public BasicShot
{
public:
	BounceShot(void);
	~BounceShot(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int		m_iBounceRange;
	float	m_fBounceModifier;

};



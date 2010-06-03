#pragma once
#include "strike.h"

class SpineCrank :
	public Strike
{
public:
	SpineCrank(void);
	~SpineCrank(void);

	bool Activate(PlayerCharacter* pSkillOwner);

protected:
	m_fBaseSpeed;
};

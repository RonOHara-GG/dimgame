#pragma once
#include "strike.h"

class ButterflyKick :
	public Strike
{
public:
	ButterflyKick(void);
	~ButterflyKick(void);

	bool Activate(Actor* pTarget, PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	float m_fAgiMultiple;
};

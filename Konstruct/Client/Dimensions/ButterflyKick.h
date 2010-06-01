#pragma once
#include "strike.h"

class ButterflyKick :
	public Strike
{
public:
	ButterflyKick(void);
	~ButterflyKick(void);

	bool Use(Actor* pTarget, PlayerCharacter* pSkillOwner);
	bool ApplyEffect(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	float m_fAgiMultiple;
};

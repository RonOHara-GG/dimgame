#pragma once
#include "strike.h"

class ReverseRoundHouse :
	public Strike
{
public:
	ReverseRoundHouse(void);
	~ReverseRoundHouse(void);

	bool ApplyEffect(PlayerCharacter* pSkillOwner, float fDeltaTime);
};

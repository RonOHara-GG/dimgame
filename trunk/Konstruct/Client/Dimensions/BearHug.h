#pragma once
#include "strike.h"

class BearHug :
	public Strike
{
public:
	BearHug(void);
	~BearHug(void);

	bool ApplyEffect(PlayerCharacter* pSkillOwner, float fDeltaTime);
};

#pragma once
#include "strike.h"

class BearHug;

class HeadButt :
	public Strike
{
public:
	HeadButt(void);
	~HeadButt(void);

	bool Use(PlayerCharacter* pSkillOwner);
	bool ApplyEffect(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	Skill* m_pCurrentBearHug;
};

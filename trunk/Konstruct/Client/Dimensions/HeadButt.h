#pragma once
#include "strike.h"

class BearHug;

class HeadButt :
	public Strike
{
public:
	HeadButt(void);
	~HeadButt(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	Skill* m_pCurrentBearHug;
};

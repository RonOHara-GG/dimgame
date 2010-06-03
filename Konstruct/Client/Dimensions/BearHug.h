#pragma once
#include "strike.h"

class BearHug :
	public Strike
{
public:
	BearHug(void);
	~BearHug(void);
	
	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);
	static int m_siBearhugIndex;

protected:	
	Actor* m_pTarget;
	float  m_fDuration;
};

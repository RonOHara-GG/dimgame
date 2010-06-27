#pragma once
#include "basicshot.h"

class ExplosiveShot :
	public BasicShot
{
public:
	ExplosiveShot(void);
	~ExplosiveShot(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);
protected:
	float	m_fRadiusMod;

};

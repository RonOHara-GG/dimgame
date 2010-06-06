#pragma once
#include "basicshot.h"

class RapidShot :
	public BasicShot
{
public:
	RapidShot(void);
	~RapidShot(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int		m_iShotCount;
	float	m_fShotMultiple;
	float	m_fSpeedMultiple;
};

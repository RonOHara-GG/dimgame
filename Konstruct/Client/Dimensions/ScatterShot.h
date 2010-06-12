#pragma once
#include "BasicShot.h"

class ScatterShot :
	public BasicShot
{
public:
	ScatterShot(void);
	~ScatterShot(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	bool	m_bTargetSelected;
	float	m_fRadius;
	float	m_fBaseDamage;
	float	m_fRadiusMod;
	float	m_fRecoveryMod;
	float	m_fSpeedMod;

};

#define MIN_RADIUS 1

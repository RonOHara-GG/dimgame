#pragma once
#include "basicshot.h"

class BurstFire :
	public BasicShot
{
public:
	BurstFire(void);
	~BurstFire(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	float		m_fDamageMod;
	float		m_fSpeedMod;
	float		m_fRecoveryMod;
	int			m_iShotsToFire;
	int			m_iShotsFired;
};


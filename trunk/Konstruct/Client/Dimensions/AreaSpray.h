#pragma once
#include "basicshot.h"

class AreaSpray :
	public BasicShot
{
public:
	AreaSpray(void);
	~AreaSpray(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int			GetRange();
	float		GetSpeed();

	float		m_fPlayerSpeed;
	float		m_fRangeMod;
	int			m_iDamage;
	float		m_fDamageMod;
	float		m_fSpeedMod;
	float		m_fRecoveryMod;
	float		m_fAngleFreq;	
	int			m_iShotsToFire;
	int			m_iShotsFired;
	int			m_iShotsMod;
};



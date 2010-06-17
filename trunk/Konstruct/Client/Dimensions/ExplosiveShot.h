#pragma once
#include "basicshot.h"

class ExplosiveShot :
	public BasicShot
{
public:
	ExplosiveShot(void);
	~ExplosiveShot(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	float m_fRadius;
	float m_fRadiusMod;
	int	m_iRadiusDamage;
	float m_fSpeedMod;
	float m_fRecoveryMod;
	DamageType m_eRadiusType;

};

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
	int m_iRadius;
	int m_iRadiusMod;
	float m_fRadiusDamage;
	float m_fSpeedMod;
	float m_fRecoveryMod;
	DamageType m_eRadiusType;

};

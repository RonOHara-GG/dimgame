#pragma once
#include "projectile.h"

class LightningProjectile :
	public Projectile
{
public:
	LightningProjectile(float fDamage, float fRange, DamageType eDamageType, Actor* pOwner, kpuVector vLocation, kpuVector vDir, float fArcChance, float fResistStr);
	~LightningProjectile(void);

protected:
	void Impact(kpuVector vImpact);

	float	m_fResistStr;
	float	m_fArcChance;
};

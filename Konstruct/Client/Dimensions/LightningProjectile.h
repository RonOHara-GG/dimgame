#pragma once
#include "projectile.h"

class LightningProjectile :
	public Projectile
{
public:
	LightningProjectile(int iDamage, float fRange, DamageType eDamageType, Actor* pOwner, kpuVector vLocation, kpuVector vDir, float fArcChance, int iResistStr);
	~LightningProjectile(void);

protected:
	void Impact(kpuVector vImpact);

	int		m_iResistStr;
	float	m_fArcChance;
};

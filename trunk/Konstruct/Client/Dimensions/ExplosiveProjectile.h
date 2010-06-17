#pragma once
#include "projectile.h"

class ExplosiveProjectile :
	public Projectile
{
public:
	ExplosiveProjectile(ProjectileType eType, int iDamage, float fRange, DamageType eDamageType, Actor* pOwner, kpuVector vLocation, kpuVector vDir,
		float fRadius, int iExplosionDmg, DamageType eExplosionType);

	~ExplosiveProjectile(void);

protected:
	void Impact(kpuVector vImpact);

	float		m_fRadius;
	int			m_iExplosionDmg;
	DamageType	m_eExplosionType;
};

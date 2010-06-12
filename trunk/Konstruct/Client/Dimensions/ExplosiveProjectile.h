#pragma once
#include "projectile.h"

class ExplosiveProjectile :
	public Projectile
{
public:
	ExplosiveProjectile(ProjectileType eType, float fDamage, float fRange, DamageType eDamageType, Actor* pOwner, kpuVector vLocation, kpuVector vDir,
		int iRadius, float fExplosionDmg, DamageType eExplosionType);

	~ExplosiveProjectile(void);

protected:
	void Impact(kpuVector vImpact);

	int			m_iRadius;
	float		m_fExplosionDmg;
	DamageType	m_eExplosionType;
};

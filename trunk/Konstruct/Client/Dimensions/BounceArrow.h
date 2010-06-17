#pragma once
#include "projectile.h"

class BounceArrow :
	public Projectile
{
public:
	BounceArrow(int iDamage, float fRange, DamageType eDamageType, Actor* pOwner, kpuVector vLocation, kpuVector vDir, float fBounceRange);
	~BounceArrow(void);	

protected:
	void Impact(kpuVector vImpact);
	float		m_fBounceRange;
};

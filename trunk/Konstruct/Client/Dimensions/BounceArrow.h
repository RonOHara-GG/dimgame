#pragma once
#include "projectile.h"

class BounceArrow :
	public Projectile
{
public:
	BounceArrow(float fDamage, float fRange, DamageType eDamageType, Actor* pOwner, kpuVector vLocation, kpuVector vDir, int iBounceRange);
	~BounceArrow(void);	

protected:
	void Impact(kpuVector vImpact);
	int		m_iBounceRange;
};

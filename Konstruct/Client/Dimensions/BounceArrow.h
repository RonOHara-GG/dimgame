#pragma once
#include "projectile.h"

class BounceArrow :
	public Projectile
{
public:
	BounceArrow(float fDamage, float fRange, DamageType eDamageType, Actor* pOwner, kpuVector vLocation, kpuVector vDir, int iBounceRange);
	~BounceArrow(void);

	bool Update(float fGameTime);

protected:
	int		m_iBounceRange;
};

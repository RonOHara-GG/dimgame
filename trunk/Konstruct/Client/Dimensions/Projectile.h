#pragma once
#include "Actor.h"

class Projectile :
	public Actor
{
public:

	enum ProjectileType
	{
		ePT_Arrow,
		ePT_Bullet,
		ePT_Spell,
		ePT_Missile
	};

	Projectile(ProjectileType eType, float fDamage, float fRange, DamageType eDamageType, Actor* pOwner, kpuVector vLocation, kpuVector vDir);
	Projectile(ProjectileType eType, float fDamage, float fRange, DamageType eDamageType, Actor* pOwner, kpuVector vLocation, kpuVector vDir, float fRadius, float fResist);
	virtual ~Projectile(void);	

	bool Update(float fGameTime);	

protected:
	virtual void Impact(kpuVector vImpact);

	ProjectileType	m_eProjectileType;
	float			m_fDamage;
	DamageType		m_eDamageType;	
	float			m_fRange;
	float			m_fRadius;
	float			m_fResistStr;
	float			m_fDistTraveled;
	Actor*			m_pOwner;
	Actor*			m_pLastHit;
};

#define ARROW_SPEED 100.0f
#define BULLET_SPEED 975.0f

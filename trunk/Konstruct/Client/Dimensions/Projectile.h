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

	Projectile(ProjectileType eType, int iDamage, float fRange, DamageType eDamageType, Actor* pOwner, kpuVector vLocation, kpuVector vDir);
	Projectile(ProjectileType eType, int iDamage, float fRange, DamageType eDamageType, Actor* pOwner, kpuVector vLocation, kpuVector vDir, float fRadius, int iResist);
	virtual ~Projectile(void);	

	bool Update(float fGameTime);	

protected:
	virtual void Impact(kpuVector vImpact);

	ProjectileType	m_eProjectileType;
	int				m_iDamage;
	DamageType		m_eDamageType;	
	float			m_fRange;
	float			m_fRadius;
	int				m_iResistStr;
	float			m_fDistTraveled;
	Actor*			m_pOwner;
	Actor*			m_pLastHit;
};

#define ARROW_SPEED 100.0f
#define BULLET_SPEED 975.0f

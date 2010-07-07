#include "StdAfx.h"
#include "ExplosiveProjectile.h"
#include "Level.h"
#include "Common/utility/kpuArrayList.h"
#include "Common/utility/kpuQuadTree.h"
#include "Common/utility/kpuCollisionData.h"
#include "Common/utility/kpuBoundingSphere.h"


ExplosiveProjectile::ExplosiveProjectile(ProjectileType eType, int iDamage, float fRange, DamageType eDamageType, Actor* pOwner, kpuVector vLocation, kpuVector vDir,
										 float fRadius, int iExplosionDmg, DamageType eExplosionType) :	Projectile(ePT_Arrow, iDamage, fRange, eDamageType, pOwner, vLocation, vDir)
{
	m_fRadius = fRadius;
	m_iExplosionDmg = iExplosionDmg;
	m_eExplosionType = eExplosionType;
}

ExplosiveProjectile::~ExplosiveProjectile(void)
{
}

void ExplosiveProjectile::Impact(kpuVector vImpact)
{	
	//hit all targets in range
	kpuBoundingSphere sphere(m_fRadius, vImpact);
	kpuArrayList<kpuCollisionData> collidedObjects;

	g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &collidedObjects);

	for(int i = 0; i < collidedObjects.Count(); i++)
	{
		kpuCollisionData* pNext = &collidedObjects[i];

		if( pNext->m_pObject->HasFlag(ENEMY) )
		{
			Actor* pTarget = (Actor*)pNext->m_pObject;

			if( pTarget->InLineOfSight(this, m_fRadius) )
			{
				pTarget->TakeDamage(m_iExplosionDmg, m_eExplosionType);
			}
		}
	}

	m_fDistTraveled = m_iRange * m_iRange;

}
#include "StdAfx.h"
#include "ExplosiveProjectile.h"
#include "Level.h"
#include "Common/utility/kpuArrayList.h"
#include "Common/utility/kpuQuadTree.h"
#include "Common/utility/kpuCollisionData.h"
#include "Common/utility/kpuBoundingSphere.h"


ExplosiveProjectile::ExplosiveProjectile(ProjectileType eType, float fDamage, float fRange, DamageType eDamageType, Actor* pOwner, kpuVector vLocation, kpuVector vDir,
										 int iRadius, float fExplosionDmg, DamageType eExplosionType) :	Projectile(ePT_Arrow, fDamage, fRange, eDamageType, pOwner, vLocation, vDir)
{
	m_iRadius = iRadius;
	m_fExplosionDmg = fExplosionDmg;
	m_eExplosionType = eExplosionType;
}

ExplosiveProjectile::~ExplosiveProjectile(void)
{
}

void ExplosiveProjectile::Impact(kpuVector vImpact)
{	
	//hit all targets in range
	kpuBoundingSphere sphere(m_iRadius, vImpact);
	kpuArrayList<kpuCollisionData> collidedObjects;

	g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &collidedObjects);

	for(int i = 0; i < collidedObjects.Count(); i++)
	{
		kpuCollisionData* pNext = &collidedObjects[i];

		pNext->m_pObject->AreaEffect(vImpact, m_iRadius, &m_fExplosionDmg, this);
	}

	m_fDistTraveled = m_fRange * m_fRange;

}
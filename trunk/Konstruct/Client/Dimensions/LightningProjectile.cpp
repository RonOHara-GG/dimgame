#include "StdAfx.h"
#include "LightningProjectile.h"
#include "Grid.h"
#include "Level.h"
#include "Common/Utility/kpuQuadTree.h"
#include "Common/Utility/kpuArrayList.h"
#include "Common/Utility/kpuCollisionData.h"
#include "Common/Utility/kpuBoundingCapsule.h"

LightningProjectile::LightningProjectile(float fDamage, float fRange, DamageType eDamageType, Actor* pOwner, kpuVector vLocation, kpuVector vDir, float fArcChance, float fResistStr) :
	Projectile(ePT_Spell, fDamage, fRange, eDamageType, pOwner, vLocation, vDir)
{
	m_fArcChance = fArcChance;
	m_fResistStr = fResistStr;
}

LightningProjectile::~LightningProjectile(void)
{
}

void LightningProjectile::Impact(kpuVector vImpact)
{
	//see if chance is triggered
	int iChance = rand() % 100;

	if( iChance < m_fArcChance )
	{
		kpuBoundingSphere sphere(m_fRange, vImpact);
		kpuArrayList<kpuCollisionData> pCollisions;
		g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &pCollisions);

		float fClosest = m_fRange * m_fRange;
		Actor* pTarget = 0;

		//find the closest target
		for(int i = 0; i < pCollisions.Count(); i++)
		{
			kpuCollisionData data = pCollisions[i];
			
			if( data.m_pObject->HasFlag(ATTACKABLE) && InLineOfSight((Actor*)data.m_pObject, m_fRange) )
			{
				float fDist = kpuVector::DistanceSquared(data.m_pObject->GetLocation(), vImpact);

				if( fDist < fClosest )
				{
					pTarget = (Actor*)data.m_pObject;
				}

				if( fDist == 1 )
				{
					//one is the closest any enemy can be
					pTarget = (Actor*)data.m_pObject;
					break;
				}

			}
		}

		if( pTarget != 0 )
		{
			m_vHeading = pTarget->GetLocation() - vImpact;
			m_vHeading.Normalize();				
		}
		else
		{
			m_fDistTraveled = m_fRange * m_fRange;
		}
	}
}

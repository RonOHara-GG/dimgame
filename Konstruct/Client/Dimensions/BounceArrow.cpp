#include "StdAfx.h"
#include "BounceArrow.h"
#include "Grid.h"
#include "Level.h"
#include "Common/Utility/kpuQuadTree.h"
#include "Common/Utility/kpuArrayList.h"
#include "Common/Utility/kpuCollisionData.h"
#include "Common/Utility/kpuBoundingCapsule.h"


#define DAMAGE_REDUCTION 0.75f
#define BOUNCE_REDUCTION 0.5f

BounceArrow::BounceArrow( float fDamage, float fRange, DamageType eDamageType, Actor* pOwner, kpuVector vLocation, kpuVector vDir, int iBounceRange) :
	Projectile(ePT_Arrow, fDamage, fRange, eDamageType, pOwner, vLocation, vDir)
{
	m_iBounceRange = iBounceRange;
}

BounceArrow::~BounceArrow(void)
{
}

void BounceArrow::Impact(kpuVector vImpact)
{
	//move arrow to this spot
	SetLocation(vImpact);

	//find new target within the range of the bounce
	kpuBoundingSphere sphere(m_iBounceRange, vImpact);
	kpuArrayList<kpuCollisionData> pCollisions;
	g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &pCollisions);

	float fClosest = m_iBounceRange * m_iBounceRange;
	Actor* pTarget = 0;

	//find the closest target
	for(int i = 0; i < pCollisions.Count(); i++)
	{
		kpuCollisionData data = pCollisions[i];
		
		if( data.m_pObject->HasFlag(ATTACKABLE) && InLineOfSight((Actor*)data.m_pObject, m_iBounceRange) )
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

		//Cut bounce range in half
		m_iBounceRange *= BOUNCE_REDUCTION;
		m_fDamage *= DAMAGE_REDUCTION;
	}
	else
	{
		//arrow bounces to nothing
		 m_fDistTraveled = m_fRange * m_fRange;
	}


}
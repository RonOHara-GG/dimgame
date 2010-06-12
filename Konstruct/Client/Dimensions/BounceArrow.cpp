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

bool BounceArrow::Update(float fGameTime)
{
	if( m_fDistTraveled >= m_fRange )
				return false;

	kpuVector vLocation = GetLocation();
	kpuVector vOldLocation = vLocation;

	//move the arrow	
	float fDist = fGameTime * ARROW_SPEED;
	kpuVector vVelocity = GetHeading() * fDist;
	m_fDistTraveled += fDist;

	//see what the arror hits and move it
	kpuBoundingCapsule capsule(vOldLocation, vOldLocation + vVelocity, 0.0f);

	kpuCollisionData data = g_pGameState->GetLevel()->GetQuadTree()->GetClosestCollision(capsule);

	if( data.m_pObject && data.m_pObject != m_pOwner && data.m_pObject != m_pLastHit )
	{
		if(  data.m_pObject->HasFlag(ATTACKABLE) )
		{
			Actor* pTarget = (Actor*)data.m_pObject;

			m_pLastHit = pTarget;
			pTarget->TakeDamage(m_fDamage, m_eDamageType);
			Impact(pTarget->GetLocation());

			//move arrow to this spot
			vLocation = vOldLocation + (GetHeading() * data.m_fDist);

			//find new target within the range of the bounce
			kpuBoundingSphere sphere(m_iBounceRange, pTarget->GetLocation());
			kpuArrayList<kpuCollisionData> pCollisions;
			g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &pCollisions);

			float fClosest = m_iBounceRange * m_iBounceRange;
			pTarget = 0;

			//find the closest target
			for(int i = 0; i < pCollisions.Count(); i++)
			{
				kpuCollisionData data = pCollisions[i];
				
				if( data.m_pObject->HasFlag(ATTACKABLE) && InLineOfSight((Actor*)data.m_pObject, m_iBounceRange) )
				{
					float fDist = kpuVector::DistanceSquared(data.m_pObject->GetLocation(), vLocation);

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
				m_vHeading = pTarget->GetLocation() - vLocation;
				m_vHeading.Normalize();				
			}
			else
			{
				//arrow bounces to nothing
				return false;
			}

			//Cut bounce range in half
			m_iBounceRange *= BOUNCE_REDUCTION;
			m_fDamage *= DAMAGE_REDUCTION;
		}
		else if( data.m_pObject->HasFlag(WALL) )
		{
			if( data.m_fDist <= 0 )			
				return false;

			SetLocation(vOldLocation + (vVelocity * (data.m_fDist / data.m_fVelLength)));
			return true;
		}
	}

	SetLocation(vLocation);
	return true;
}
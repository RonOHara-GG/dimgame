#include "StdAfx.h"
#include "BounceShot.h"
#include "Weapon.h"
#include "PlayerCharacter.h"
#include "Grid.h"
#include "Level.h"
#include "Common/Utility/kpuQuadTree.h"
#include "Common/Utility/kpuArrayList.h"
#include "Common/Utility/kpuCollisionData.h"

#define MIN_BOUNCE_RANGE 2

BounceShot::BounceShot(void)
{
}

BounceShot::~BounceShot(void)
{
}

bool BounceShot::Activate(PlayerCharacter *pSkillOwner)
{
	if( m_bReady )
	{
		m_fRange = pSkillOwner->GetEquippedWeapon()->GetRange();
		m_fDamage = pSkillOwner->GetEquippedWeapon()->GetDamage() * (m_fDamageMultiple * ( m_fDamageMultiple * m_iSkillRank * m_fRankMultipleMin ));
		m_iBounceRange = MIN_BOUNCE_RANGE + ( m_iSkillRank * m_fBounceModifier );

		m_bReady = false;
		m_bExecuted = false;
		m_fDistTraveled = 0.0f;
		m_fElaspedSinceCast = 0.0f;
		m_vLocation = pSkillOwner->GetLocation();
		m_pLastHit = 0;
		return true;
	}

	return false;
}

bool BounceShot::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	if( m_iBounceRange == 0 )
	{
		m_fElaspedSinceCast = 0.0f;
		return false;
	}

	if( m_fDistTraveled >= m_fRange )
		return false;

	int iTile = g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(m_vLocation);

	Actor* pTarget = g_pGameState->GetLevel()->GetGrid()->GetActor(iTile);

	if( pTarget )
	{
		if( pTarget->Attackable() && pTarget != pSkillOwner && pTarget != m_pLastHit )
		{
			m_pLastHit = pTarget;

			pTarget->TakeDamage(m_fDamage, m_pEquippedWeapon->GetDamageType());

			//find new target 
			kpuBoundingSphere sphere(m_iBounceRange, m_vLocation);
			kpuArrayList<kpuCollisionData> pCollisions;
			g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &pCollisions);

			float fClosest = m_iBounceRange * m_iBounceRange;
			pTarget = 0;

			//find the closest target
			for(int i = 0; i < pCollisions.Count(); i++)
			{
				kpuCollisionData data = pCollisions[i];
				
				if( !data.m_pObject->IsStatic() )
				{
					float fDist = kpuVector::DistanceSquared(data.m_pObject->GetLocation(), pSkillOwner->GetLocation());

					if( fDist < fClosest )
					{
						pTarget = (Actor*)data.m_pObject;
					}

					if( fDist == 1 )
					{
						//one is the closest any enemy can be
						break;
					}

				}

			}

			if( pTarget != 0 )
			{
				m_vDirection = pTarget->GetLocation() - m_vLocation;
				m_vDirection.Normalize();				
			}
			else
			{
				//arrow bounces to nothing
				return false;
			}

			//Cut bounce range in half
			m_iBounceRange /= 2;

		}

	}

	//see what the arror hits and move it
	kpuBoundingSphere sphere(0.5f, m_vLocation);

	if( g_pGameState->GetLevel()->GetQuadTree()->CheckCollision(sphere, pSkillOwner) )
	{
		//arrow collides with wall return false
		m_fElaspedSinceCast = 0.0f;
		return false;
	}

	




}
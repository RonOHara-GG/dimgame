#include "StdAfx.h"
#include "ScatterShot.h"
#include "Weapon.h"
#include "PlayerCharacter.h"
#include "Level.h"
#include "grid.h"
#include "Common/input/kpiInputManager.h"
#include "Common/utility/kpuArrayList.h"
#include "Common/utility/kpuQuadTree.h"
#include "Common/utility/kpuCollisionData.h"
#include "Common/utility/kpuBoundingSphere.h"

ScatterShot::ScatterShot(void)
{
}

ScatterShot::~ScatterShot(void)
{
}


bool ScatterShot::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	//shoot when animation is ready
	if( true )
	{
		m_bExecuted = true;

		Weapon* pEquippedWeapon = pSkillOwner->GetEquippedWeapon();

		int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );

		int iBaseDamage = pEquippedWeapon->GetDamage() * m_iDamageMultiple + (iRankMultiple * m_iSkillRank);
		
		m_fRecovery = pEquippedWeapon->GetRecovery() * m_fRecoveryMod;

		float fRadius = m_fMinRadius + (m_iSkillRank * m_fRadiusMod);

		kpuVector vTarget = kpuv_Zero;
		g_pGameState->GetLevel()->GetGrid()->GetTileLocation(m_iTargetTile, vTarget);

		//get all actors in range
		kpuBoundingSphere sphere(fRadius, vTarget);
		kpuArrayList<kpuCollisionData> collidedObjects;

		g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &collidedObjects);

		for(int i = 0; i < collidedObjects.Count(); i++)
		{
			kpuCollisionData* pNext = &collidedObjects[i];

			//get distance to the object
			float fDist = (pNext->m_pObject->GetLocation() - vTarget).Length();

			int iDamage = iBaseDamage - int( (fDist / fRadius) * iBaseDamage );
			
			if( pNext->m_pObject->HasFlag(ENEMY) )
			{
				Actor* pTarget = (Actor*)pNext->m_pObject;
				
				if( pTarget->InLineOfSight(vTarget, GetRange()) )
					pTarget->TakeDamage(iDamage, pEquippedWeapon->GetDamageType());
			}
		}

	}


	return true;
}
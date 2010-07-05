#include "StdAfx.h"
#include "Paralyze.h"
#include "PlayerCharacter.h"
#include "Enemy.h"
#include "Level.h"
#include "grid.h"
#include "PlayerPet.h"
#include "Common/utility/kpuArrayList.h"
#include "Common/utility/kpuQuadTree.h"
#include "Common/utility/kpuCollisionData.h"
#include "Common/utility/kpuBoundingSphere.h"

Paralyze::Paralyze(void)
{
}

Paralyze::~Paralyze(void)
{
}


bool Paralyze::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{			
	//wait for animation
	if( true )
	{
		//get enemys in radius		
		float fRadius = m_fMinRadius + (m_iSkillRank / m_iRadiusMod);

		kpuVector vTarget;
		g_pGameState->GetLevel()->GetGrid()->GetTileLocation(m_iTargetTile, vTarget);

		kpuBoundingSphere sphere(fRadius, vTarget);
		kpuArrayList<kpuCollisionData> collidedObjects;

		g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &collidedObjects);

		for(int i = 0; i < collidedObjects.Count(); i++)
		{
			kpuCollisionData* pNext = &collidedObjects[i];
			
			if( pNext->m_pObject->HasFlag(ENEMY) )
			{
				Enemy* pTarget = (Enemy*)pNext->m_pObject;

				int iResistStr = m_iMinResist + (m_iSkillRank * m_iResistMod);

				if( pTarget->InLineOfSight(vTarget, fRadius, pSkillOwner) && pTarget->GetResist(m_eDamageType) < iResistStr )
				{						
					pTarget->SetSpeed(0.0f);
					pTarget->SetActionRange(0.0f);
				}

			}		
			
		}
		m_bExecuted = true;
		return false;
	}	

	return true;
}
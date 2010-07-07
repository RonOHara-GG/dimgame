#include "StdAfx.h"
#include "ChangeAllegiance.h"
#include "PlayerCharacter.h"
#include "Enemy.h"
#include "Level.h"
#include "Grid.h"
#include "PlayerPet.h"
#include "Common/utility/kpuArrayList.h"
#include "Common/utility/kpuQuadTree.h"
#include "Common/utility/kpuCollisionData.h"
#include "Common/utility/kpuBoundingSphere.h"

ChangeAllegiance::ChangeAllegiance(void)
{
}

ChangeAllegiance::~ChangeAllegiance(void)
{
}

bool ChangeAllegiance::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	//Go with animation
	if( true )
	{
		float fRadius = m_fMinRadius + (m_iSkillRank / m_fRadiusMod);
		int iResistStr = m_iMinResist + (m_iSkillRank * m_iResistMod);	

		kpuVector vTarget = kpuv_Zero;
		g_pGameState->GetLevel()->GetGrid()->GetTileLocation(m_iTargetTile, vTarget);

		//get enemys in radius		
		kpuBoundingSphere sphere(fRadius, vTarget);
		kpuArrayList<kpuCollisionData> collidedObjects;

		g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &collidedObjects);

		for(int i = 0; i < collidedObjects.Count(); i++)
		{
			kpuCollisionData* pNext = &collidedObjects[i];
			
			if( pNext->m_pObject->HasFlag(ENEMY) )
			{
				Enemy* pTarget = (Enemy*)pNext->m_pObject;

				if( pTarget->InLineOfSight(vTarget, fRadius, pSkillOwner) && pTarget->GetResist(m_eDamageType) < iResistStr )
				{						
					pSkillOwner->AddPet(new PlayerPet(pTarget, pSkillOwner));	
					pTarget->SetCurrentHealth(-1);
				}

			}		
			
		}
		m_bExecuted = true;
		return false;
	}
	

	return true;
}

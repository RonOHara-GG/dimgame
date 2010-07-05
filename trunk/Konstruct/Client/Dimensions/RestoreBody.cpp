#include "StdAfx.h"
#include "RestoreBody.h"
#include "PlayerCharacter.h"
#include "Level.h"
#include "Common/utility/kpuArrayList.h"
#include "Common/utility/kpuQuadTree.h"
#include "Common/utility/kpuCollisionData.h"
#include "Common/utility/kpuBoundingSphere.h"

RestoreBody::RestoreBody(void)
{
}

RestoreBody::~RestoreBody(void)
{
}

bool RestoreBody::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if( m_fElaspedSinceCast >= m_fSpeed )
	{
		m_bExecuted = true;
		float fRadius = m_fMinRadius + m_iSkillRank * m_fRadiusMod;
		
		int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );
		int iHeal = iRankMultiple * m_iSkillRank;

		//get all actors in range
		kpuBoundingSphere sphere(fRadius, pSkillOwner->GetLocation());
		kpuArrayList<kpuCollisionData> collidedObjects;

		g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &collidedObjects);

		for(int i = 0; i < collidedObjects.Count(); i++)
		{
			kpuCollisionData* pNext = &collidedObjects[i];

			//heal the target
			if( pNext->m_pObject->HasFlag(PLAYER) )
			{
				PlayerCharacter* pPlayer = (PlayerCharacter*)pNext->m_pObject;

				//check line of sight
				if( pPlayer->InLineOfSight(pSkillOwner, fRadius) )
					pPlayer->Heal(iHeal);
			}

				
		}

		return true;
	}

	return false;
}
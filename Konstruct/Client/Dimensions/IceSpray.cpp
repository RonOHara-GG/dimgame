#include "StdAfx.h"
#include "IceSpray.h"
#include "PlayerCharacter.h"
#include "Level.h"
#include "Common/utility/kpuArrayList.h"
#include "Common/utility/kpuQuadTree.h"
#include "Common/utility/kpuCollisionData.h"
#include "Common/utility/kpuBoundingSphere.h"

#define MIN_ARC 0.0872664626f

IceSpray::IceSpray(void)
{
}

IceSpray::~IceSpray(void)
{
}

bool IceSpray::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	
	if( true )
	{
		int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );
		int iDamage = iRankMultiple * m_iSkillRank;
		int iResistStr = m_iMinResist + m_iSkillRank * m_iResistMod;

		//spray some ice
		//get all actors in range
		//find the enemies in the arc
		float fHalfTheta = MIN_ARC + (m_iSkillRank * m_fArcMod);
		fHalfTheta *= 0.5f;

		
		//possible add bounding cone
		kpuBoundingSphere sphere(GetRange(), pSkillOwner->GetLocation());
		kpuArrayList<kpuCollisionData> collidedObjects;

		g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &collidedObjects);

		for(int i = 0; i < collidedObjects.Count(); i++)
		{
			kpuCollisionData* pNext = &collidedObjects[i];

			//get distance to the object
			kpuVector vDir = pNext->m_pObject->GetLocation() - pSkillOwner->GetLocation();
			vDir.Normalize();
			
			float fBeta = atan2(vDir.GetX(), vDir.GetZ());		
			

			if( fBeta <= fHalfTheta || fBeta >= PI * 2 - fHalfTheta )
			{
				if( pNext->m_pObject->HasFlag(ENEMY) )
				{
					Actor* pTarget = (Actor*)pNext->m_pObject;

					if( pTarget->InLineOfSight(pSkillOwner, GetRange()) )
						pTarget->TakeDamage(iDamage, m_eDamageType, iResistStr);
				}
			}
		}
		

		m_bExecuted = true;
		return false;
	}

	return true;
}
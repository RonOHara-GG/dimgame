#include "StdAfx.h"
#include "SpreadableBuff.h"
#include "PlayerCharacter.h"
#include "Level.h"
#include "Common/utility/kpuArrayList.h"
#include "Common/utility/kpuQuadTree.h"
#include "Common/utility/kpuCollisionData.h"
#include "Common/utility/kpuBoundingSphere.h"

SpreadableBuff::SpreadableBuff(PlayerCharacter* pSource)
{
	m_pSource = pSource;
	m_fElaspedSinceCast = 0.0f;
	m_fSpeed = 1.0f;
	m_pInfected = 0;

	Activate(pSource);
}

SpreadableBuff::SpreadableBuff(void)
{
}

SpreadableBuff::~SpreadableBuff(void)
{
}

bool SpreadableBuff::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	//make sure still in range of source
	float fDist = kpuVector::DistanceSquared(pSkillOwner->GetLocation(), m_pSource->GetLocation());

	if( fDist > m_fRadius * m_fRadius )
		return false;

	if( pSkillOwner == m_pSource )
	{
		//try and spread it
		//find a new target and shoot it
		kpuBoundingSphere sphere(m_fRadius, pSkillOwner->GetLocation());
		
		kpuArrayList<kpuCollisionData> aCollisions;

		g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &aCollisions);

		for(int i = 0; i < aCollisions.Count(); i++)
		{
			kpuCollisionData* pNext = &aCollisions[i];

			if( pNext->m_pObject->HasFlag(PLAYER) )
			{
				PlayerCharacter* pTarget = (PlayerCharacter*)pNext->m_pObject;
				
				if( pTarget->InLineOfSight(pSkillOwner, m_fRadius * 2) && !pTarget->HasPersistentSkill(m_uID))
				{
					SpreadableBuff* pCopy =CopyBuff(pTarget);
					pCopy->Activate(pTarget);
					pTarget->AddPersistentSkill(pCopy);										
				}				
			}
		}

	}

	m_fElaspedSinceCast += fDeltaTime;

	if( m_fElaspedSinceCast >= m_fSpeed )
	{
		Tick(pSkillOwner);
		m_fElaspedSinceCast = 0.0f;
	}

	return true;
}


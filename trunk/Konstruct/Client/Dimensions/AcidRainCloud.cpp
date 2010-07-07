#include "StdAfx.h"
#include "AcidRainCloud.h"
#include "PlayerCharacter.h"
#include "Level.h"
#include "Common/utility/kpuArrayList.h"
#include "Common/utility/kpuQuadTree.h"
#include "Common/utility/kpuCollisionData.h"
#include "Common/utility/kpuBoundingSphere.h"

AcidRainCloud::AcidRainCloud(kpuVector vLoc, int iDamage, DamageType eDT, float fRadius, int iResist)
{
	m_iDamage = iDamage;
	m_eDamageType = eDT;
	m_fRadius = fRadius;
	m_iResistStr = iResist;
	m_vLocation = vLoc;

	m_fTickTime = 1.0f;
	m_fTickElasped = 0.0f;
}

AcidRainCloud::~AcidRainCloud(void)
{
}

bool AcidRainCloud::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	m_fTickElasped += fDeltaTime;

	if( m_fTickElasped >= m_fTickTime )
	{
		m_fTickElasped = 0.0f;

		//get all actors in range
		kpuBoundingSphere sphere(m_fRadius, m_vLocation);
		kpuArrayList<kpuCollisionData> collidedObjects;

		g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &collidedObjects);

		for(int i = 0; i < collidedObjects.Count(); i++)
		{
			kpuCollisionData* pNext = &collidedObjects[i];	

			//make sure it is an enemy
			if( pNext->m_pObject->HasFlag(ENEMY) )			
				((Actor*)pNext->m_pObject)->TakeDamage(m_iDamage, m_eDamageType, m_iResistStr);			
		}
	}

	return true;
}
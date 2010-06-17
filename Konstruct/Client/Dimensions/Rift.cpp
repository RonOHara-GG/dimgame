#include "StdAfx.h"
#include "Rift.h"
#include "PlayerCharacter.h"
#include "Enemy.h"
#include "Level.h"
#include "Common/utility/kpuArrayList.h"
#include "Common/utility/kpuQuadTree.h"
#include "Common/utility/kpuCollisionData.h"
#include "Common/utility/kpuBoundingSphere.h"

Rift::Rift(PlayerCharacter* pOwner, int iLevel, float fRadius, kpuVector vLocation, float fResist, float fDuration, DamageType eType):
	PlayerPet(pOwner, iLevel)
{
	m_fDuration = fDuration;
	m_fRadius = fRadius;
	m_iResistStr = fResist;
	SetLocation(vLocation);
	m_fElasped = 0.0f;
}

Rift::~Rift(void)
{
}

bool Rift::Update(float fGameTime)
{
	m_fElasped += fGameTime;

	if( m_fElasped >= m_fDuration )
	{
		m_pOwner->RemovePet(this);
		return false;
	}

	//get all actors in range
	kpuBoundingSphere sphere(m_fRadius, GetLocation());
	kpuArrayList<kpuCollisionData> collidedObjects;

	g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &collidedObjects);

	for(int i = 0; i < collidedObjects.Count(); i++)
	{
		kpuCollisionData* pNext = &collidedObjects[i];
		
		if( pNext->m_pObject->HasFlag(ENEMY) )
		{
			Enemy* pTarget = (Enemy*)pNext->m_pObject;

			if( pTarget->InLineOfSight(this, m_fRadius) && pTarget->GetResist(m_eDamageType) < m_iResistStr )
				pTarget->SetCurrentHealth(-1);
		}		
		
	}

	return true;
}

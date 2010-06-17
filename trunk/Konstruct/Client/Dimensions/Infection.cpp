#include "StdAfx.h"
#include "Infection.h"
#include "PlayerCharacter.h"
#include "Level.h"
#include "Common/utility/kpuArrayList.h"
#include "Common/utility/kpuQuadTree.h"
#include "Common/utility/kpuCollisionData.h"
#include "Common/utility/kpuBoundingSphere.h"

Infection::Infection(void)
{
}

Infection::Infection(Actor* pTarget, float fDamage, float fResist, float fInfectRadius, DamageType eDamageType)
	:PersistentSkill(pTarget)
{
	m_fDamage = fDamage;	
	m_fResistStr = fResist;
	m_fInfectRadius = fInfectRadius;
	m_eDamageType = eDamageType;

	m_fTickTime = 1.0f;
	m_fTickElasped = 1.0f;
	m_pTarget->SetResist(m_pTarget->GetResist(m_eDamageType) - m_fResistStr, m_eDamageType);
}

Infection::~Infection(void)
{
	m_pTarget->SetResist(m_pTarget->GetResist(m_eDamageType) + m_fResistStr, m_eDamageType);
}

bool Infection::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	m_fTickElasped += fDeltaTime;

	if( m_fTickElasped >= m_fTickTime )
	{
		m_fTickElasped = 0.0f;

		//deal damage
		if( !m_pTarget->TakeDamage(m_fDamage, m_eDamageType) )
		{
			//resisted so terminate the skill
			m_pTarget->RemovePersistentSkill(this);
			return false;
		}
	}

	//try and infect near by enemies
	kpuBoundingSphere sphere(m_fInfectRadius, m_pTarget->GetLocation());
		
	kpuArrayList<kpuCollisionData> aCollisions;

	g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &aCollisions);

	for(int i = 0; i < aCollisions.Count(); i++)
	{
		kpuCollisionData* pNext = &aCollisions[i];

		if( pNext->m_pObject->HasFlag(ATTACKABLE) )
		{
			Actor* pTarget = (Actor*)pNext->m_pObject;

			if( pTarget->InLineOfSight(m_pTarget, m_fInfectRadius * 2) )
			{
				pTarget->AddPersistentSkill(new Infection(pTarget, m_fDamage, m_fResistStr, m_fInfectRadius, m_eDamageType));
			}
		}
	}

	return true;
}

#include "StdAfx.h"
#include "PlagueCenser.h"
#include "Infection.h"
#include "Level.h"
#include "Common/utility/kpuArrayList.h"
#include "Common/utility/kpuQuadTree.h"
#include "Common/utility/kpuCollisionData.h"
#include "Common/utility/kpuBoundingSphere.h"


PlagueCenser::PlagueCenser(kpuVector vLocation, int iDamage, DamageType eDamageType, int iResistStr, float fRadius)
{
	m_iDamage = iDamage;
	m_eDamageType = eDamageType;
	m_iResistStr = iResistStr;
	m_fRadius = fRadius;
	m_fTickTime = 1.0f;
	m_fElaspedTick = 1.0f;
	m_iCurrentHealth = 1;
	SetLocation(vLocation);
}

PlagueCenser::~PlagueCenser(void)
{
}

bool PlagueCenser::Update(float fGameTime)
{
	m_fElaspedTick += fGameTime;

	if( m_fElaspedTick >= m_fTickTime )
	{
		m_fElaspedTick = 0.0f;
		//spread infection

		kpuBoundingSphere sphere(m_fRadius, GetLocation());
		
		kpuArrayList<kpuCollisionData> aCollisions;

		g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &aCollisions);

		for(int i = 0; i < aCollisions.Count(); i++)
		{
			kpuCollisionData* pNext = &aCollisions[i];

			if( pNext->m_pObject->HasFlag(ATTACKABLE) )
			{
				Actor* pTarget = (Actor*)pNext->m_pObject;

				if( pTarget->InLineOfSight(this, m_fRadius * 2) )
				{
					pTarget->AddPersistentSkill(new Infection(pTarget, m_iDamage, m_iResistStr, 0, m_eDamageType));
				}
			}
		}
	}

	if( m_iCurrentHealth < 0 )
		return false;

	return true;
}
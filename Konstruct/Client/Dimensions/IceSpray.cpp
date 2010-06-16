#include "StdAfx.h"
#include "IceSpray.h"
#include "PlayerCharacter.h"
#include "Level.h"
#include "Common/utility/kpuArrayList.h"
#include "Common/utility/kpuQuadTree.h"
#include "Common/utility/kpuCollisionData.h"
#include "Common/utility/kpuBoundingSphere.h"

#define MIN_ARC 0.0872664626f
#define MIN_RANGE 2
#define MIN_RESIST 7

IceSpray::IceSpray(void)
{
}

IceSpray::~IceSpray(void)
{
}

bool IceSpray::Activate(PlayerCharacter *pSkillOwner)
{
	if( m_bReady )
	{
		m_fRange = MIN_RANGE + m_iSkillRank /m_fRangeMod;
		m_fMaxCosSqrd = cos(MIN_ARC + (m_iSkillRank * m_fArcMod));

		m_fMaxCosSqrd *= m_fMaxCosSqrd;
		
		int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );
		m_fDamage = iRankMultiple * m_iSkillRank;
		m_fResistStr = MIN_RESIST + m_iSkillRank * m_fResistMod;

		m_fElaspedSinceCast = 0.0f;
		m_bReady = false;
		m_bExecuted = false;

		pSkillOwner->SetActiveSkill(this);
		return true;
	}

	return false;
}

bool IceSpray::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if( m_fElaspedSinceCast >= m_fSpeed )
	{
		//spray some ice
		//get all actors in range
		//find the enemies in the arc
		kpuBoundingSphere sphere(m_fRange, pSkillOwner->GetLocation());
		kpuArrayList<kpuCollisionData> collidedObjects;

		g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &collidedObjects);

		for(int i = 0; i < collidedObjects.Count(); i++)
		{
			kpuCollisionData* pNext = &collidedObjects[i];

			//get distance to the object
			kpuVector vDir = pNext->m_pObject->GetLocation() - pSkillOwner->GetLocation();
			vDir.Normalize();

			float fCos = vDir.Dot(pSkillOwner->GetHeading());
			fCos *= fCos;

			if( fCos <= m_fMaxCosSqrd )
			{		
				pNext->m_pObject->AreaEffect(pSkillOwner->GetLocation(), m_fRange, &m_fDamage, this);
			}
		}
		

		m_bExecuted = true;
		return true;
	}

	return false;
}
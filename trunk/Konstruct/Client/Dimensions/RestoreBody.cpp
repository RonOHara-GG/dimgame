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

bool RestoreBody::Activate(PlayerCharacter *pSkillOwner)
{
	if( m_bReady )
	{
		m_fRadius = m_fMinRadius + m_iSkillRank * m_fRadiusMod;
		
		int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );
		m_iHeal = iRankMultiple * m_iSkillRank;
		
		m_fElaspedSinceCast = 0.0f;
		m_bReady = false;
		m_bExecuted = false;

		pSkillOwner->SetActiveSkill(this);
		return true;
	}

	return false;
}

bool RestoreBody::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if( m_fElaspedSinceCast >= m_fSpeed )
	{
		m_bExecuted = true;

		//get all actors in range
		kpuBoundingSphere sphere(m_fRadius, pSkillOwner->GetLocation());
		kpuArrayList<kpuCollisionData> collidedObjects;

		g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &collidedObjects);

		for(int i = 0; i < collidedObjects.Count(); i++)
		{
			kpuCollisionData* pNext = &collidedObjects[i];

			//heal the target somehow
			//pNext->m_pObject->AreaEffect(vTarget, m_fRadius, &m_fHeal, this);
		}

		return true;
	}

	return false;
}
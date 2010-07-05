#include "StdAfx.h"
#include "DivineRetribution.h"
#include "PlayerCharacter.h"
#include "Enemy.h"
#include "Level.h"
#include "Common/utility/kpuArrayList.h"
#include "Common/utility/kpuQuadTree.h"
#include "Common/utility/kpuCollisionData.h"
#include "Common/utility/kpuBoundingSphere.h"


DivineRetribution::DivineRetribution(PlayerCharacter* pSource, PlayerCharacter* pSpreadTo, float fRadius):
	SpreadableBuff(pSource)
{
	m_pSpreadTo = pSpreadTo;
	m_fRadius = fRadius;
}

DivineRetribution::DivineRetribution(void)
{
}

DivineRetribution::~DivineRetribution(void)
{
}

bool DivineRetribution::Activate(PlayerCharacter *pSkillOwner)
{
	m_fRadius = m_fMinRadius + (m_iSkillRank / m_fRadiusMod);
	return true;
}

bool DivineRetribution::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );
	int iDamage = iRankMultiple * m_iSkillRank;

	//make sure still in range of source
	float fDist = kpuVector::DistanceSquared(pSkillOwner->GetLocation(), m_pSource->GetLocation());

	if( fDist > m_fRadius * m_fRadius )
		return false;

	//check enemies and their last attacked target to try and see who dealt the damage	
		
	kpuBoundingSphere sphere(m_fRadius, m_pSpreadTo->GetLocation());
	
	kpuArrayList<kpuCollisionData> aCollisions;

	g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &aCollisions);

	for(int i = 0; i < aCollisions.Count(); i++)
	{
		kpuCollisionData* pNext = &aCollisions[i];

		if( pSkillOwner == m_pSource && pNext->m_pObject->HasFlag(PLAYER) )
		{
			PlayerCharacter* pTarget = (PlayerCharacter*)pNext->m_pObject;
			
			if( pTarget->InLineOfSight(pSkillOwner, m_fRadius * 2) )
			{
				DivineRetribution* pCopy = new DivineRetribution(m_pSource, pTarget, m_fRadius);
				pTarget->AddPersistentSkill(pCopy);									
			}				
		}
		else if( pNext->m_pObject->HasFlag(ENEMY) )
		{
			Enemy* pEnemy = (Enemy*)pNext->m_pObject;

			if( pEnemy->GetLastAttacked() == m_pSpreadTo )
				pEnemy->TakeDamage(iDamage, m_eDamageType);
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
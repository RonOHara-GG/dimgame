#include "StdAfx.h"
#include "AcidRain.h"
#include "PlayerCharacter.h"
#include "Level.h"
#include "Common/utility/kpuArrayList.h"
#include "Common/utility/kpuQuadTree.h"
#include "Common/utility/kpuCollisionData.h"
#include "Common/utility/kpuBoundingSphere.h"

#define MIN_SPEED 5
AcidRain::AcidRain(void)
{
}

AcidRain::~AcidRain(void)
{
}

bool AcidRain::Activate(PlayerCharacter* pSkillOwner)
{
	if(m_bReady)
	{
		m_fElaspedSinceCast = 0.0f;
		m_fElaspedSinceTick = 0.0f;

		int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );

		m_iDamage = iRankMultiple * m_iSkillRank;
		m_fRange = m_fMinRange + m_iSkillRank /  m_iRangeMod;
		m_fSpeed =  MIN_SPEED + m_iSkillRank * m_fSpeedMod;		
		m_fRadius = m_fMinRadius + (m_iSkillRank / m_iRadiusMod);
		m_iResistStr = m_fMinResist + (m_iSkillRank * m_iResistMod);		

		pSkillOwner->SetActiveSkill(this);
		
		m_bReady = false;
		m_bExecuted = false;	
		m_bTargetSelected = false;
		
		return true;		
	}

	return false;
}

bool AcidRain::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	//check input and see if mouse was clicked

	//if left mouse clicked then target selected and get the target	

	//if right mouse clicked then cancel skill
	//return false;


	if( m_bTargetSelected )
	{
		m_fElaspedSinceCast += fDeltaTime;
		m_fElaspedSinceTick += fDeltaTime;

		//Good time to execute?
		if( m_fElaspedSinceTick >= 1.0f  )
		{
			m_bExecuted = true;
			m_fElaspedSinceTick = 0.0f;

			//get all actors in range
			kpuBoundingSphere sphere(m_fRadius, m_vTarget);
			kpuArrayList<kpuCollisionData> collidedObjects;

			g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &collidedObjects);

			for(int i = 0; i < collidedObjects.Count(); i++)
			{
				kpuCollisionData* pNext = &collidedObjects[i];				
				
				pNext->m_pObject->AreaEffect(m_vTarget, m_fRadius, &m_iDamage, this);
			}

		}

		if( m_fElaspedSinceCast >= m_fSpeed )
			return false;
	}

	return true;
}
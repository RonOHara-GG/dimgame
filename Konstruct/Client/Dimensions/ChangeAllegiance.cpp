#include "StdAfx.h"
#include "ChangeAllegiance.h"
#include "PlayerCharacter.h"
#include "Enemy.h"
#include "Level.h"
#include "PlayerPet.h"
#include "Common/utility/kpuArrayList.h"
#include "Common/utility/kpuQuadTree.h"
#include "Common/utility/kpuCollisionData.h"
#include "Common/utility/kpuBoundingSphere.h"

ChangeAllegiance::ChangeAllegiance(void)
{
}

ChangeAllegiance::~ChangeAllegiance(void)
{
}

bool ChangeAllegiance::Activate(PlayerCharacter* pSkillOwner)
{
	if(m_bReady)
	{
		m_fElaspedSinceCast = 0.0f;		
		
		m_fRange = m_fMinRange + m_iSkillRank /  m_iRangeMod;
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

bool ChangeAllegiance::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	//check input and see if mouse was clicked

	//if left mouse clicked then target selected and get the target	

	//if right mouse clicked then cancel skill
	//return false;


	if( m_bTargetSelected )
	{
		m_fElaspedSinceCast += fDeltaTime;		

		if( m_fElaspedSinceCast >= m_fSpeed )
		{
			//get enemys in radius		
			kpuBoundingSphere sphere(m_fRadius, m_vTarget);
			kpuArrayList<kpuCollisionData> collidedObjects;

			g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &collidedObjects);

			for(int i = 0; i < collidedObjects.Count(); i++)
			{
				kpuCollisionData* pNext = &collidedObjects[i];
				
				if( pNext->m_pObject->HasFlag(ENEMY) )
				{
					Enemy* pTarget = (Enemy*)pNext->m_pObject;

					if( pTarget->InLineOfSight(m_vTarget, m_fRadius, pSkillOwner) && pTarget->GetResist(m_eDamageType) < m_iResistStr )
					{						
						pSkillOwner->AddPet(new PlayerPet(pTarget, pSkillOwner));	
						pTarget->SetCurrentHealth(-1);
					}

				}		
				
			}
			m_bExecuted = true;
			return false;
		}
	}

	return true;
}

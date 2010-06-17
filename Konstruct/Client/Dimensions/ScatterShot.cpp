#include "StdAfx.h"
#include "ScatterShot.h"
#include "Weapon.h"
#include "PlayerCharacter.h"
#include "Level.h"
#include "Common/utility/kpuArrayList.h"
#include "Common/utility/kpuQuadTree.h"
#include "Common/utility/kpuCollisionData.h"
#include "Common/utility/kpuBoundingSphere.h"

ScatterShot::ScatterShot(void)
{
}

ScatterShot::~ScatterShot(void)
{
}

bool ScatterShot::Activate(PlayerCharacter* pSkillOwner)
{
	if(m_bReady)
	{
		Weapon* pEquippedWeapon = pSkillOwner->GetEquippedWeapon();

		m_fElaspedSinceCast = 0.0f;

		int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );

		m_iBaseDamage = pEquippedWeapon->GetDamage() * m_iDamageMultiple + (iRankMultiple * m_iSkillRank);
		m_fRange = pEquippedWeapon->GetRange() * m_fRangeMultiple;
		m_fSpeed = pEquippedWeapon->GetSpeed() * m_fSpeedMod;
		m_fRecovery = pEquippedWeapon->GetRecovery() * m_fRecoveryMod;
		m_fRadius = MIN_RADIUS + (m_iSkillRank * m_fRadiusMod);
		m_eDamageType = pEquippedWeapon->GetDamageType();

		pSkillOwner->SetActiveSkill(this);
		
		m_bReady = false;
		m_bExecuted = false;	
		m_bTargetSelected = false;
		
		return true;		
	}

	return false;
}

bool ScatterShot::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	//check input and see if mouse was clicked

	//if left mouse clicked then target selected and get the target	

	//if right mouse clicked then cancel skill
	//return false;


	if( m_bTargetSelected )
	{
		m_fElaspedSinceCast += fDeltaTime;

		//Good time to execute?
		if( !m_bExecuted && m_fElaspedSinceCast >= m_fSpeed * 0.75f )
		{
			m_bExecuted = true;

			//get all actors in range
			kpuBoundingSphere sphere(m_fRadius, m_vTarget);
			kpuArrayList<kpuCollisionData> collidedObjects;

			g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &collidedObjects);

			for(int i = 0; i < collidedObjects.Count(); i++)
			{
				kpuCollisionData* pNext = &collidedObjects[i];

				//get distance to the object
				float fDist = (pNext->m_pObject->GetLocation() - m_vTarget).Length();

				m_iDamage = m_iBaseDamage - ( (fDist / m_fRadius) * m_iBaseDamage );
				
				pNext->m_pObject->AreaEffect(m_vTarget, m_fRadius, &m_iDamage, this);
			}

		}

		if( m_fElaspedSinceCast >= m_fSpeed )
			return false;
	}

	return true;
}
#include "StdAfx.h"
#include "MultiShot.h"
#include "Weapon.h"
#include "PlayerCharacter.h"
#include "Grid.h"
#include "Level.h"
#include "Common/Utility/kpuQuadTree.h"
#include "Common/Utility/kpuMatrix.h"

MultiShot::MultiShot(void)
{
}

MultiShot::~MultiShot(void)
{
}

bool MultiShot::Activate(PlayerCharacter *pSkillOwner)
{
	if(m_bReady)
	{
		m_pEquippedWeapon = pSkillOwner->GetEquippedWeapon();

		m_fElaspedSinceCast = 0.0f;		

		m_iShotsFired = MIN_SHOTS_FIRED + ( m_iSkillRank * m_fNumOfShotsMultiple );
		m_fDamage = m_pEquippedWeapon->GetDamage() + (m_iShotsFired - (pSkillOwner->GetInt() * m_fIntMultiple));
		m_fRange = m_pEquippedWeapon->GetRange();
		
		m_fFireArc = m_fFireArcMultiple * m_iSkillRank;

		pSkillOwner->SetActiveSkill(this);
		
		m_bReady = false;
		m_bExecuted = false;
		m_fDistTraveled = 0.0f;
		
		m_vLocation = pSkillOwner->GetLocation();

		//find the number of radians between each shot
		float fRadiansPerShot = m_fFireArc / m_iShotsFired;
		fRadiansPerShot *= DEG_TO_RAD;
		kpuVector vDir = pSkillOwner->GetHeading() % kpuv_OneY;

		//fill in the data for all the arrows being fired
		for(int i = 0; i < m_iShotsFired; i++ )
		{
			kpuMatrix rotMatrix;
			rotMatrix.Identity();

			rotMatrix.SetRotationY( (i + 1) * fRadiansPerShot);
			m_vaLocations[i] = m_vLocation;
			m_vaDirections[i] = vDir * rotMatrix;
			m_paLastActorsHit = 0;
		}

		return true;		
	}

	return false;


}

bool MultiShot::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	float fDist = fDeltaTime * ARROW_SPEED;

	if( m_fDistTraveled >= m_fRange )
		return false;

	for(int i = 0; i < m_iShotsFired; i++)
	{
		kpuVector vLocation = m_vaLocations[i];
		Actor* pLastHit = m_paLastActorsHit[i];

		int iTile = g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(vLocation);

		Actor* pTarget = g_pGameState->GetLevel()->GetGrid()->GetActor(iTile);

		//check if we shot something
		if( pTarget )
		{
			if( pTarget->Attackable() && pTarget != pSkillOwner && pTarget != pLastHit )
			{
				m_paLastActorsHit[i] = pTarget;

				pTarget->TakeDamage(m_fDamage, m_pEquippedWeapon->GetDamageType());
			}
		}

		//see what the arror hits and move it
		kpuBoundingSphere sphere(0.5f, m_vLocation);

		if( g_pGameState->GetLevel()->GetQuadTree()->CheckCollision(sphere, pSkillOwner) )
		{
			//arrow collided with wall shift the arrrow data down
			
			for(int j = i; j < m_iShotsFired - 1; j++)
			{
				m_vaLocations[j] = m_vaLocations[j+ 1];
				m_paLastActorsHit[j] = m_paLastActorsHit[j + 1];
				m_vaDirections[j] = m_vaDirections[j + 1];
			}

			m_iShotsFired--;
			
		}
		else //move the arrow	
			m_vLocation += m_vaDirections[i] * fDist;
	}

	m_fDistTraveled += fDist;

	return true;
}

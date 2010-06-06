#include "StdAfx.h"
#include "BasicShot.h"
#include "Weapon.h"
#include "PlayerCharacter.h"
#include "Grid.h"
#include "Level.h"
#include "Common/Utility/kpuQuadTree.h"


BasicShot::BasicShot(void)
{
}

BasicShot::~BasicShot(void)
{
}

bool BasicShot::Activate(PlayerCharacter *pSkillOwner)
{	
	if(m_bReady)
	{
		m_pEquippedWeapon = pSkillOwner->GetEquippedWeapon();

		m_fElaspedSinceCast = 0.0f;

		int iRankMultiple = m_fRankMultipleMin + ( rand() % (int)(m_fRankMultipleMax - m_fRankMultipleMin) );

		m_fDamage = m_pEquippedWeapon->GetDamage() + (iRankMultiple * m_iSkillRank);
		m_fRange = m_pEquippedWeapon->GetRange() + (m_iSkillRank * m_fRangeMultiple);

		pSkillOwner->SetActiveSkill(this);
		
		m_bReady = false;
		m_bExecuted = false;
		m_fDistTraveled = 0.0f;
		
		m_vLocation = pSkillOwner->GetLocation();
		m_pLastHit = 0;
		return true;		
	}

	return false;
}

bool BasicShot::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	if( m_fDistTraveled >= m_fRange )
		return false;

	int iTile = g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(m_vLocation);

	Actor* pTarget = g_pGameState->GetLevel()->GetGrid()->GetActor(iTile);

	if( pTarget )
	{
		if( pTarget->Attackable() && pTarget != pSkillOwner && pTarget != m_pLastHit )
		{
			m_pLastHit = pTarget;

			pTarget->TakeDamage(m_fDamage, m_pEquippedWeapon->GetDamageType());

		}

	}

	//see what the arror hits and move it
	kpuBoundingSphere sphere(0.5f, m_vLocation);

	if( g_pGameState->GetLevel()->GetQuadTree()->CheckCollision(sphere, pSkillOwner) )
	{
		//arrow collides with wall return false
		m_fElaspedSinceCast = 0.0f;
		return false;
	}

	//move the arrow
	float fDist = fDeltaTime * ARROW_SPEED;
	m_vLocation += m_vDirection * fDist;
	m_fDistTraveled += fDist;
}
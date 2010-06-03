#include "StdAfx.h"
#include "Strike.h"
#include "PlayerCharacter.h"
#include "Grid.h"
#include "Level.h"
#include "Common/Utility/kpuQuadTree.h"

Strike::Strike(void)
{
	m_iRankMultipleMin = 0;
	m_iRankMultipleMax = 0;
	m_iRange = 0;
	m_iRadius = 0;	
	m_eDamageType = eDT_Crushing;
	m_fSpeed = 0.0f;
	m_fContactTime = 0.0f;
	m_fStrMultiple = 0.5f;
}

Strike::~Strike(void)
{

}

bool Strike::Activate(PlayerCharacter *pSkillOwner)
{
	if(m_bReady)
	{
		m_fElaspedSinceCast = 0.0f;

		int iRankMultiple = m_iRankMultipleMin + ( rand() % (m_iRankMultipleMax - m_iRankMultipleMin) );

		m_fDamage = ( (iRankMultiple * m_iSkillRank) + ( pSkillOwner->GetStr() * m_fStrMultiple ));

		pSkillOwner->SetActiveSkill(this);
		
		m_bReady = false;
		return true;		
	}

	return false;
}

bool Strike::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if(m_fElaspedSinceCast >= m_fContactTime)
	{
		//Get the target right in front of the player within the attack range	
		int iTile = g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(pSkillOwner->GetLocation() + ( pSkillOwner->GetHeading() * m_iRange ) );

		Actor* pTarget = g_pGameState->GetLevel()->GetGrid()->GetActor(iTile);

		if( pTarget )
		{
			if( pTarget->Attackable() )
			{
				pTarget->TakeDamage(m_fDamage, m_eDamageType);
			}
		}	

		if( m_fElaspedSinceCast >= m_fSpeed )
		{
			m_fElaspedSinceCast = 0.0f;
			return true;
		}
	}

	return false;
}

void Strike::MoveTarget(Actor *pTarget, kpuVector vDirection, float fSpeed)
{
	int iStartTile = g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(pTarget->GetLocation());
	kpuVector vStart;

	g_pGameState->GetLevel()->GetGrid()->GetTileLocation(iStartTile, vStart);

	//Get Target tile
	int iTargetTile =  g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(vStart + vDirection);
	kpuVector vTarget;
	g_pGameState->GetLevel()->GetGrid()->GetTileLocation(iTargetTile, vTarget);

	//See if wall is in the way
	kpuBoundingSphere sphere(0.5f, vTarget);
	if( g_pGameState->GetLevel()->GetQuadTree()->CheckCollision(sphere, pTarget) )
	{
		//enemy collided with wall so no change in tile
		return;
	}

	pTarget->SetLocation(pTarget->GetLocation() + ( vDirection * fSpeed ));

}
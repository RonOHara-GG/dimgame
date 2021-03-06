#include "StdAfx.h"
#include "Strike.h"
#include "PlayerCharacter.h"
#include "Grid.h"
#include "Level.h"
#include "Common/Utility/kpuQuadTree.h"

Strike::Strike(void)
{
	m_iRankMultipleMin = 1;
	m_iRankMultipleMax = 2;
	m_iRange = 1;
	m_iRadius = 1;	
	m_eDamageType = eDT_Crushing;
	m_fSpeed = 0.5f;
	m_fContactTime = 0.25f;
	m_fStrMultiple = 0.5f;
	m_bExecuted = false;
}

Strike::~Strike(void)
{

}

bool Strike::Activate(PlayerCharacter *pSkillOwner)
{
	if(m_bReady)
	{
		m_fElaspedSinceCast = 0.0f;

		

		pSkillOwner->SetActiveSkill(this);
		
		m_bReady = false;
		m_bExecuted = false;
		return true;		
	}

	return false;
}

bool Strike::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	if( true )
	{
		//Get the target right in front of the player within the attack range	
		int iTile = g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(pSkillOwner->GetLocation() + ( pSkillOwner->GetHeading() * (float)m_iRange ) );

		Actor* pTarget = g_pGameState->GetLevel()->GetGrid()->GetActor(iTile);

		if( pTarget )
		{
			if( pTarget->HasFlag(ATTACKABLE) )
			{
				int iRankMultiple = m_iRankMultipleMin + ( rand() % (m_iRankMultipleMax - m_iRankMultipleMin) );
				int iDamage = (iRankMultiple * m_iSkillRank) + int( pSkillOwner->GetStr() * m_fStrMultiple );

				pTarget->TakeDamage(iDamage, m_eDamageType);
			}
		}	

		return false;

	}

	return true;;
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

	pTarget->Move(vDirection * fSpeed );

}
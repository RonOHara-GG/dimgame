#include "StdAfx.h"
#include "ReverseRoundHouse.h"
#include "PlayerCharacter.h"
#include "Grid.h"
#include "Level.h"

ReverseRoundHouse::ReverseRoundHouse(void)
{
}

ReverseRoundHouse::~ReverseRoundHouse(void)
{
}

bool ReverseRoundHouse::ApplyEffect(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if(m_fElaspedSinceCast >= m_fSpeed)
	{
		//Hit all targets in front arc
		Grid* pGrid = g_pGameState->GetLevel()->GetGrid();
		int iStartTile = pGrid->GetTileAtLocation(pSkillOwner->GetLocation());

		kpuVector vStart;
		pGrid->GetTileLocation(iStartTile, vStart);

		int iTargetTile = pGrid->GetTileAtLocation(pSkillOwner->GetHeading());

		Actor* pTarget = pGrid->GetActor(iTargetTile);

		if( pTarget )	
			pTarget->TakeDamage(m_fDamage, m_eDamageType);

		//Get the next target to the right of the player
		kpuVector vDirection = pSkillOwner->GetHeading() % kpuv_OneY;
		iTargetTile = pGrid->GetTileAtLocation(vStart + vDirection);

		pTarget = pGrid->GetActor(iTargetTile);

		if( pTarget )	
			pTarget->TakeDamage(m_fDamage, m_eDamageType);

		//Get upper right target
		iTargetTile = pGrid->GetTileAtLocation(vStart + vDirection + pSkillOwner->GetHeading());

		pTarget = pGrid->GetActor(iTargetTile);

		if( pTarget )	
			pTarget->TakeDamage(m_fDamage, m_eDamageType);

		//Get the target to the left
		vDirection *= -1;
		iTargetTile = pGrid->GetTileAtLocation(vStart + vDirection);

		pTarget = pGrid->GetActor(iTargetTile);

		if( pTarget )	
			pTarget->TakeDamage(m_fDamage, m_eDamageType);

		//Get upper left target
		iTargetTile = pGrid->GetTileAtLocation(vStart + vDirection + pSkillOwner->GetHeading());

		pTarget = pGrid->GetActor(iTargetTile);

		if( pTarget )	
			pTarget->TakeDamage(m_fDamage, m_eDamageType);

		m_fElaspedSinceCast = 0.0f;
		return true;
	}

	return false;

}



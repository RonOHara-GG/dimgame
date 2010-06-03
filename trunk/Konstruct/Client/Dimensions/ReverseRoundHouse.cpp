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

bool ReverseRoundHouse::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	//Hit all targets in front arc
	Grid* pGrid = g_pGameState->GetLevel()->GetGrid();
	int iStartTile = pGrid->GetTileAtLocation(pSkillOwner->GetLocation());

	kpuVector vStart;
	pGrid->GetTileLocation(iStartTile, vStart);

	int iTargetTile; // = pGrid->GetTileAtLocation(pSkillOwner->GetHeading());

	Actor* pTarget = 0; // = pGrid->GetActor(iTargetTile);

	if(m_fElaspedSinceCast >= m_fContactTime )
	{
		kpuVector vDirection = pSkillOwner->GetHeading() % kpuv_OneY;
		vDirection *= -1;

		iTargetTile = pGrid->GetTileAtLocation(vStart + vDirection);

		if( pTarget )	
			pTarget->TakeDamage(m_fDamage, m_eDamageType);
	}
	else if( m_fElaspedSinceCast >= m_fContactTime * 2 )
	{

		//Get upper left target
		kpuVector vDirection = pSkillOwner->GetHeading() % kpuv_OneY;
		vDirection *= -1;
		
		iTargetTile = pGrid->GetTileAtLocation(vStart + vDirection + pSkillOwner->GetHeading());

		pTarget = pGrid->GetActor(iTargetTile);

		if( pTarget )	
			pTarget->TakeDamage(m_fDamage, m_eDamageType);
	}
	else if( m_fElaspedSinceCast >= m_fContactTime * 3 )
	{
		iTargetTile = pGrid->GetTileAtLocation(pSkillOwner->GetHeading());
		pTarget = pGrid->GetActor(iTargetTile);

		if( pTarget )	
			pTarget->TakeDamage(m_fDamage, m_eDamageType);

	}
	else if( m_fElaspedSinceCast >= m_fContactTime * 4 )
	{
		//Get upper right target
		kpuVector vDirection = pSkillOwner->GetHeading() % kpuv_OneY;
		iTargetTile = pGrid->GetTileAtLocation(vStart + vDirection + pSkillOwner->GetHeading());

		pTarget = pGrid->GetActor(iTargetTile);

		if( pTarget )	
			pTarget->TakeDamage(m_fDamage, m_eDamageType);
	}
	else if( m_fElaspedSinceCast >= m_fContactTime * 5 )
	{
		//Get upper right target
		kpuVector vDirection = pSkillOwner->GetHeading() % kpuv_OneY;
		iTargetTile = pGrid->GetTileAtLocation(vStart + vDirection);

		pTarget = pGrid->GetActor(iTargetTile);		

		if( pTarget )	
			pTarget->TakeDamage(m_fDamage, m_eDamageType);
		
	}
	else if( m_fElaspedSinceCast >= m_fContactTime * 6 )
	{
		m_fElaspedSinceCast = 0.0f;
		return true;
	}

	return false;

}



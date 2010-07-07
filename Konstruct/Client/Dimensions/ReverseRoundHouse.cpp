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

bool ReverseRoundHouse::Activate(PlayerCharacter *pSkillOwner)
{
	if( Skill::Activate(pSkillOwner) )
	{		
		m_eCurrentTarget = e_drLEFT;
		return true;
	}

	return false;
}

bool ReverseRoundHouse::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if( m_fElaspedSinceCast >= m_fContactTime * 6 )
	{
		m_fElaspedSinceCast = 0.0f;
		return false;
	}

	//Hit all targets in front arc
	Grid* pGrid = g_pGameState->GetLevel()->GetGrid();
	int iStartTile = pGrid->GetTileAtLocation(pSkillOwner->GetLocation());

	kpuVector vStart;
	pGrid->GetTileLocation(iStartTile, vStart);

	int iTargetTile; // = pGrid->GetTileAtLocation(pSkillOwner->GetHeading());

	Actor* pTarget = 0; // = pGrid->GetActor(iTargetTile);

	int iRankMultiple = m_iRankMultipleMin + ( rand() % (m_iRankMultipleMax - m_iRankMultipleMin) );
	int iDamage = (iRankMultiple * m_iSkillRank) + int( pSkillOwner->GetStr() * m_fStrMultiple );

	switch( m_eCurrentTarget )
	{
	case e_drLEFT:
		if(m_fElaspedSinceCast >= m_fContactTime )
		{
			//Left target
			kpuVector vDirection = pSkillOwner->GetHeading() % kpuv_OneY;	
			vDirection *= -1;

			iTargetTile = pGrid->GetTileAtLocation(vStart + vDirection);
			pTarget = pGrid->GetActor(iTargetTile);

			if( pTarget )	
				pTarget->TakeDamage(iDamage, m_eDamageType);

			m_eCurrentTarget = e_drUPPER_LEFT;		
		}
		break;

	case e_drUPPER_LEFT:
		if( m_fElaspedSinceCast >= m_fContactTime * 2 )
		{

			//Get upper left target
			kpuVector vDirection = pSkillOwner->GetHeading() % kpuv_OneY;
			vDirection *= -1;
			
			iTargetTile = pGrid->GetTileAtLocation(vStart + vDirection + pSkillOwner->GetHeading());

			pTarget = pGrid->GetActor(iTargetTile);

			if( pTarget )	
				pTarget->TakeDamage(iDamage, m_eDamageType);

			m_eCurrentTarget = e_drFRONT;		
		}
		break;

	case e_drFRONT:
		if( m_fElaspedSinceCast >= m_fContactTime * 3 )
		{
			//Front target
			iTargetTile = pGrid->GetTileAtLocation(vStart + pSkillOwner->GetHeading());
			pTarget = pGrid->GetActor(iTargetTile);

			if( pTarget )	
				pTarget->TakeDamage(iDamage, m_eDamageType);

			m_eCurrentTarget = e_drUPPER_RIGHT;		
		}
		break;

	case e_drUPPER_RIGHT:
		if( m_fElaspedSinceCast >= m_fContactTime * 4 )
		{
			//Get upper right target
			kpuVector vDirection = pSkillOwner->GetHeading() % kpuv_OneY;
		
			iTargetTile = pGrid->GetTileAtLocation(vStart + vDirection + pSkillOwner->GetHeading());

			pTarget = pGrid->GetActor(iTargetTile);

			if( pTarget )	
				pTarget->TakeDamage(iDamage, m_eDamageType);
			m_eCurrentTarget = e_drRIGHT;		
		}
		break;

	case e_drRIGHT:
		if( m_fElaspedSinceCast >= m_fContactTime * 5 )
		{
			//Get upper right target
			kpuVector vDirection = pSkillOwner->GetHeading() % kpuv_OneY;
			
			iTargetTile = pGrid->GetTileAtLocation(vStart + vDirection);

			pTarget = pGrid->GetActor(iTargetTile);		

			if( pTarget )	
				pTarget->TakeDamage(iDamage, m_eDamageType);

			m_eCurrentTarget = e_drNONE;		
		}
		break;
	}

	

	return true;

}



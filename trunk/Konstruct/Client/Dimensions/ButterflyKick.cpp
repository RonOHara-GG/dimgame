#include "StdAfx.h"
#include "ButterflyKick.h"
#include "playercharacter.h"
#include "Grid.h"
#include "Level.h"

#define MOVE_DIST 3

ButterflyKick::ButterflyKick(void)
{
	m_fAgiMultiple = 0.0f;
}

ButterflyKick::~ButterflyKick(void)
{
}


bool ButterflyKick::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if( m_fElaspedSinceCast >= m_fSpeed )
	{
		//move to center of current tile
		int iCurrent = g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(pSkillOwner->GetLocation());
		pSkillOwner->SetNextMove(iCurrent);
		m_fElaspedSinceCast = 0.0f;
		m_iLastTile = -1;
		return false;
	}

	int iCurrentTile = g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(pSkillOwner->GetLocation());
	int iRankMultiple = m_iRankMultipleMin + ( rand() % (m_iRankMultipleMax - m_iRankMultipleMin) );
	int iDamage = (iRankMultiple * m_iSkillRank) + int( pSkillOwner->GetStr() * m_fStrMultiple );

	if( iCurrentTile != m_iLastTile )
	{
		//Get the target in front of the player and to his right then move the player forward	
		kpuVector vDir = pSkillOwner->GetHeading() % kpuv_OneY;

		//tile to the right
		int iTile = g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(pSkillOwner->GetLocation() +  vDir);

		//deal damage to right tile
		Actor* pTarget = g_pGameState->GetLevel()->GetGrid()->GetActor(iTile);

		if( pTarget && pTarget->HasFlag(ATTACKABLE) )
		{
			pTarget->TakeDamage(iDamage , m_eDamageType);
		}

		//deal half damage to front tile and move it to the left
		iTile = g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation( pSkillOwner->GetLocation() +  pSkillOwner->GetHeading() );

		pTarget = g_pGameState->GetLevel()->GetGrid()->GetActor(iTile);

		if( pTarget && pTarget->HasFlag(ATTACKABLE) )
		{
			pTarget->TakeDamage(iDamage / 2 , m_eDamageType);

			pTarget->BuildStraightPath( 1.0f, vDir * -1);		
		}		
		
		m_iLastTile = iCurrentTile;
	}

	pSkillOwner->Move(pSkillOwner->GetHeading() * ( fDeltaTime / m_fSpeed ) * MOVE_DIST);

	return true;
}
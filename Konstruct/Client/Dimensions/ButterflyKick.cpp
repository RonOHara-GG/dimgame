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

bool ButterflyKick::Activate(Actor *pTarget, PlayerCharacter *pSkillOwner)
{
	if(m_bReady)
	{
		//Check if target is attack able		
		int iRankMultiple = m_iRankMultipleMin + ( rand() % (m_iRankMultipleMax - m_iRankMultipleMin) );
		m_fDamage = ( (iRankMultiple * m_iSkillRank) + ( pSkillOwner->GetStr() * m_fStrMultiple ) + ( m_fAgiMultiple * pSkillOwner->GetAgi()) );

		pSkillOwner->SetActiveSkill(this);
		m_iLastTile = -1;
		
		m_bReady = false;
		return true;			
		
	}

	return false;
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
		return false;
	}

	int iCurrentTile = g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(pSkillOwner->GetLocation());

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
			pTarget->TakeDamage(m_fDamage , m_eDamageType);
		}

		//deal half damage to front tile and move it to the left
		iTile = g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation( pSkillOwner->GetLocation() +  pSkillOwner->GetHeading() );

		pTarget = g_pGameState->GetLevel()->GetGrid()->GetActor(iTile);

		if( pTarget && pTarget->HasFlag(ATTACKABLE) )
		{
			pTarget->TakeDamage(m_fDamage * 0.5f , m_eDamageType);

			MoveTarget(pTarget, vDir * -1, 1.0f);
		}		
		
		m_iLastTile = iCurrentTile;
	}

	pSkillOwner->Move(pSkillOwner->GetHeading() * ( fDeltaTime / m_fSpeed ) * MOVE_DIST);

	return true;
}
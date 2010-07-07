#include "StdAfx.h"
#include "BearHug.h"
#include "PlayerCharacter.h"
#include "Grid.h"
#include "Level.h"

int BearHug::m_siBearhugIndex = 0;

BearHug::BearHug(void)
{
	m_pTarget = 0;
}

BearHug::~BearHug(void)
{
}

bool BearHug::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	if( !m_pTarget )
	{
		Grid* pGrid = g_pGameState->GetLevel()->GetGrid();
		int iTile = pGrid->GetTileAtLocation(pSkillOwner->GetLocation() + ( pSkillOwner->GetHeading() * (float)m_iRange ) );
		Actor* pTarget = pGrid->GetActor(iTile);

		if( !pTarget || !pTarget->HasFlag(ENEMY) )		
			return false;
		
		m_pTarget = pTarget;
		m_fDuration = m_fSpeed + pSkillOwner->GetStr() / 25.0f;		
	}

	m_fElaspedSinceCast += fDeltaTime;

	if(m_fElaspedSinceCast > m_fDuration )
	{
		m_fElaspedSinceCast = 0.0f;
		return false;
	}

	int iRankMultiple = m_iRankMultipleMin + ( rand() % (m_iRankMultipleMax - m_iRankMultipleMin) );
	int iDamage = (iRankMultiple * m_iSkillRank) + int( pSkillOwner->GetStr() * m_fStrMultiple );

 	m_pTarget->TakeDamage(iDamage, m_eDamageType);

	if( !m_pTarget->IsAlive() )
	{
		m_fElaspedSinceCast = 0.0f;
		return false;
	}

	return true;
}

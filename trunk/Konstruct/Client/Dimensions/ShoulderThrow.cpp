#include "StdAfx.h"
#include "ShoulderThrow.h"
#include "playercharacter.h"
#include "Grid.h"
#include "Level.h"

ShoulderThrow::ShoulderThrow(void)
{
	
}

ShoulderThrow::~ShoulderThrow(void)
{
}


bool ShoulderThrow::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	//Get target
	Actor* pTarget = g_pGameState->GetLevel()->GetGrid()->GetActor(g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(pSkillOwner->GetLocation() + pSkillOwner->GetHeading()));
	float fThrowDist = m_fMinThrow + (pSkillOwner->GetStr() / m_iStrFactor);

	if( true )
	{
		if( pTarget && pTarget->HasFlag(ENEMY) )
		{
			int iRankMultiple = m_iRankMultipleMin + ( rand() % (m_iRankMultipleMax - m_iRankMultipleMin) );
			int iDamage = (iRankMultiple * m_iSkillRank) + int( pSkillOwner->GetStr() * m_fStrMultiple );

			//move target to center of current tile
			pTarget->BuildStraightPath(fThrowDist, pSkillOwner->GetHeading() * -1.0f);			
			pTarget->TakeDamage(iDamage, m_eDamageType);
		}

		m_fElaspedSinceCast = 0.0f;
		return false;
	}

	return true;
}

#include "StdAfx.h"
#include "SpineCrank.h"
#include "playercharacter.h"
#include "Grid.h"
#include "Level.h"

SpineCrank::SpineCrank(void)
{
	m_fBaseSpeed = 0.0f;
}

SpineCrank::~SpineCrank(void)
{
}

bool SpineCrank::Activate(PlayerCharacter *pSkillOwner)
{
	if( Strike::Activate(pSkillOwner) )
	{
		m_fSpeed = m_fBaseSpeed + (pSkillOwner->GetStr() / 50 );

		//see if we can instantly kill enemy
		int iTargetTile = g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(pSkillOwner->GetLocation() + pSkillOwner->GetHeading());

		m_pTarget = g_pGameState->GetLevel()->GetGrid()->GetActor(iTargetTile);

		if( !m_pTarget && !m_pTarget->HasFlag(ATTACKABLE) )
		{
			m_bReady = true;
			return false;
		}
		int iDeathChance = rand() % 2;

		if( iDeathChance == 1 )
		{
			m_pTarget->TakeDamage(m_pTarget->GetMaxHealth() * 10000, eDT_Death);
			m_fElaspedSinceCast = m_fSpeed;
		}
		
		return true;
	}

	return false;
}

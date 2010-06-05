#include "StdAfx.h"
#include "ShoulderThrow.h"
#include "playercharacter.h"
#include "Grid.h"
#include "Level.h"

ShoulderThrow::ShoulderThrow(void)
{
	m_iThrowDist = 0;
}

ShoulderThrow::~ShoulderThrow(void)
{
}

bool ShoulderThrow::Activate(PlayerCharacter* pSkillOwner)
{
	if( Strike::Activate(pSkillOwner) )
	{
		m_iThrowDist = BASE_THROW_DIST + (pSkillOwner->GetStr() / m_iStrFactor);
		return true;
	}

	return false;
}

bool ShoulderThrow::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	//Get target
	Actor* pTarget = g_pGameState->GetLevel()->GetGrid()->GetActor(g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(pSkillOwner->GetLocation() + pSkillOwner->GetHeading()));

	if( m_fElaspedSinceCast >= m_fSpeed )
	{
		if( pTarget && pTarget->Attackable() )
		{
			//move target to center of current tile
			pTarget->SetNextMove(g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(pTarget->GetLocation()));
			pTarget->TakeDamage(m_fDamage, m_eDamageType);
		}
		m_fElaspedSinceCast = 0.0f;
		return false;
	}

	if( pTarget && pTarget->Attackable() )
	{
		//try and move the target back the tiles needed, be thrown at 1m/s
		MoveTarget(pTarget, pSkillOwner->GetHeading() * -1, (fDeltaTime / m_fSpeed) * m_iThrowDist );
	}

	return true;
}

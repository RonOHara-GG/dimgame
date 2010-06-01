#include "StdAfx.h"
#include "BearHug.h"
#include "PlayerCharacter.h"
#include "Grid.h"
#include "Level.h"

BearHug::BearHug(void)
{
}

BearHug::~BearHug(void)
{
}

bool BearHug::ApplyEffect(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if(m_fElaspedSinceCast < m_fSpeed + pSkillOwner->GetStr() / 25.0f)
	{
		//Get the target right in front of the player within the attack range	
		int iTile = g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(pSkillOwner->GetLocation() + ( pSkillOwner->GetHeading() * m_iRange ) );

		Actor* pTarget = g_pGameState->GetLevel()->GetGrid()->GetActor(iTile);

		if( pTarget )
		{
			if( pTarget->Attackable() )
			{
				pTarget->TakeDamage(m_fDamage * fDeltaTime, m_eDamageType);
			}
		}	

		
		return false;
	}

	m_fElaspedSinceCast = 0.0f;
	return true;



}

#include "StdAfx.h"
#include "Strike.h"
#include "PlayerCharacter.h"
#include "Grid.h"
#include "Level.h"

Strike::Strike(void)
{
	m_iRankMultiple = 0;
	m_iRange = 0;
	m_iRadius = 0;	
	m_eDamageType = eDT_Crushing;
	m_fSpeed = 0.0f;
	m_fStrMultiple = 0.5f;
}

Strike::~Strike(void)
{

}

bool Strike::Use(PlayerCharacter *pSkillOwner)
{
	if(m_bReady)
	{
		m_fDamage = ( (m_iRankMultiple * m_iSkillRank) + ( pSkillOwner->GetStr() * m_fStrMultiple ));

		pSkillOwner->SetActiveSkill(this);
		
		m_bReady = false;
		return true;		
	}

	return false;
}

bool Strike::ApplyEffect(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if(m_fElaspedSinceCast >= m_fSpeed)
	{
		//Get the target right in front of the player within the attack range	
		int iTile = g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(pSkillOwner->GetLocation() + ( pSkillOwner->GetHeading() * m_iRange ) );

		Actor* pTarget = g_pGameState->GetLevel()->GetGrid()->GetActor(iTile);

		if( pTarget )
		{
			if( pTarget->Attackable() )
			{
				pTarget->TakeDamage(m_fDamage, m_eDamageType);
			}
		}	

		m_fElaspedSinceCast = 0.0f;
		return true;
	}

	return false;
}

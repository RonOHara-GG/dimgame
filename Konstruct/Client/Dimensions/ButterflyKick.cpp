#include "StdAfx.h"
#include "ButterflyKick.h"
#include "playercharacter.h"
#include "Grid.h"
#include "Level.h"

ButterflyKick::ButterflyKick(void)
{
	m_fAgiMultiple = 0.0f;
}

ButterflyKick::~ButterflyKick(void)
{
}

bool ButterflyKick::Use(Actor *pTarget, PlayerCharacter *pSkillOwner)
{
	if(m_bReady)
	{
		//Check if target is attack able		
		m_fDamage = ( (m_iRankMultiple * m_iSkillRank) + ( pSkillOwner->GetStr() * m_fStrMultiple ) + ( m_fAgiMultiple * pSkillOwner->GetAgi()) );

		pSkillOwner->SetActiveSkill(this);
		
		m_bReady = false;
		return true;			
		
	}

	return false;
}

bool ButterflyKick::ApplyEffect(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	//Get the target in front of the player and to his right then move the player forward
	if( pSkillOwner->AtDestination() )
	{
		//tile to the right
		int iTile = g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(pSkillOwner->GetLocation() +  ( pSkillOwner->GetHeading() % kpuv_OneY ));

		//deal half damage to front target and move to safe tile
		Actor* pTarget = g_pGameState->GetLevel()->GetGrid()->GetActor(iTile);

		if( pTarget && pTarget->Attackable() )
		{
			pTarget->TakeDamage(m_fDamage , m_eDamageType);
		}

		//Get front tile
		iTile = g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(pSkillOwner->GetLocation() +  pSkillOwner->GetHeading() );

		pTarget = g_pGameState->GetLevel()->GetGrid()->GetActor(iTile);

		if( pTarget && pTarget->Attackable() )
		{
			pTarget->TakeDamage(m_fDamage , m_eDamageType);

			//try and move the target to a nearby tile, if not possible then no more kick
			int iTargetTile = g_pGameState->GetLevel()->GetGrid()->BestMove(pTarget->GetHeading(), iTile);

			if( iTargetTile < 0 )
			{
				//move failed kick over
			}

			
			


		}



	}
	return false;
}
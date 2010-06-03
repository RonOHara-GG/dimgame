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

bool BearHug::Activate(PlayerCharacter *pSkillOwner)
{
	if( Strike::Activate(pSkillOwner) )
	{
		m_fDuration = m_fSpeed + pSkillOwner->GetStr() / 25.0f;

		int iTile = g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(pSkillOwner->GetLocation() + ( pSkillOwner->GetHeading() * m_iRange ) );

		Actor* pTarget = g_pGameState->GetLevel()->GetGrid()->GetActor(iTile);

		if( !pTarget || !pTarget->Attackable() )		
			return false;
		
		m_pTarget = pTarget;

	}

}

bool BearHug::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if(m_fElaspedSinceCast > m_fDuration)
	{
		m_fElaspedSinceCast = 0.0f;
		return true;
	}

	m_pTarget->TakeDamage(m_fDamage, m_eDamageType);
}

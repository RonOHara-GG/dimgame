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

		if( !pTarget || !pTarget->HasFlag(ATTACKABLE) )		
			return false;
		
		m_pTarget = pTarget;

		pSkillOwner->SetActiveSkill(this);
		return true;

	}

	return false;
}

bool BearHug::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if(m_fElaspedSinceCast > m_fDuration )
	{
		m_fElaspedSinceCast = 0.0f;
		return false;
	}

 	m_pTarget->TakeDamage(m_iDamage * fDeltaTime, m_eDamageType);

	if( !m_pTarget->IsAlive() )
	{
		m_fElaspedSinceCast = 0.0f;
		return false;
	}

	return true;
}

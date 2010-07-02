#include "StdAfx.h"
#include "LightningBolt.h"
#include "LightningProjectile.h"
#include "PlayerCharacter.h"

LightningBolt::LightningBolt(void)
{
}

LightningBolt::~LightningBolt(void)
{
}

bool LightningBolt::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if( m_fElaspedSinceCast >= m_fSpeed )
	{
		Projectile* pBolt= new LightningProjectile(m_iDamage, m_fRange, m_eDamageType, pSkillOwner, pSkillOwner->GetLocation(), pSkillOwner->GetHeading(), m_fMinArcChance + m_iSkillRank, m_iResistStr);
		g_pGameState->AddActor(pBolt);

		m_bExecuted = true;
		return true;
	}

	return false;
}
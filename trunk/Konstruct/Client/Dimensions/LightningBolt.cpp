#include "StdAfx.h"
#include "LightningBolt.h"
#include "LightningProjectile.h"
#include "PlayerCharacter.h"

#define MIN_ARC_CHANCE 5
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
		//shoot fireball
		Projectile* pBolt= new LightningProjectile(m_fDamage, m_fRange, m_eDamageType, pSkillOwner, pSkillOwner->GetLocation(), pSkillOwner->GetHeading(), m_fArcChance, m_fResistStr);
		g_pGameState->AddActor(pBolt);

		m_bExecuted = true;
		return true;
	}

	return false;
}

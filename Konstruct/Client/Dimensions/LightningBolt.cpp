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
	//Go on animation
	if( true )
	{
		float fRadius = m_fMinRadius + (m_iSkillRank / m_fRadiusMod);
		
		int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );
		int iDamage = iRankMultiple * m_iSkillRank;
		int iResistStr = m_iMinResist + m_iSkillRank * m_iResistMod;

		Projectile* pBolt= new LightningProjectile(iDamage, GetRange(), m_eDamageType, pSkillOwner, pSkillOwner->GetLocation(), pSkillOwner->GetHeading(), m_fMinArcChance + m_iSkillRank, iResistStr);
		g_pGameState->AddActor(pBolt);

		m_bExecuted = true;
		return true;
	}

	return false;
}

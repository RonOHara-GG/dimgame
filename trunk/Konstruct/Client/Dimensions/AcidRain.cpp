#include "StdAfx.h"
#include "AcidRain.h"
#include "AcidRainCloud.h"
#include "PlayerCharacter.h"
#include "Level.h"
#include "Grid.h"

AcidRain::AcidRain(void)
{
}

AcidRain::~AcidRain(void)
{
}

bool AcidRain::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	if( true  )
	{
		int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );

		int iDamage = iRankMultiple * m_iSkillRank;	
		float fRadius = m_fMinRadius + (m_iSkillRank / m_fRadiusMod);
		int iResistStr = m_iMinResist + (m_iSkillRank * m_iResistMod);	

		kpuVector vTarget = kpuv_Zero;
		g_pGameState->GetLevel()->GetGrid()->GetTileLocation(m_iTargetTile, vTarget);

		pSkillOwner->AddPersistentSkill(new AcidRainCloud(vTarget, iDamage, m_eDamageType, fRadius, iResistStr));
		return false;

	}
	return true;
}
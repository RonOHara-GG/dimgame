#include "StdAfx.h"
#include "PlagueCenserSkill.h"
#include "PlagueCenser.h"
#include "PlayerCharacter.h"
#include "Level.h"
#include "Grid.h"

PlagueCenserSkill::PlagueCenserSkill(void)
{
}

PlagueCenserSkill::~PlagueCenserSkill(void)
{
}

bool PlagueCenserSkill::Activate(PlayerCharacter* pSkillOwner)
{
	if( m_bReady )
	{
		m_bReady = false;
		m_bExecuted = false;
		m_fElaspedSinceCast = 0.0f;

		

		pSkillOwner->SetActiveSkill(this);

		return true;
	}

	return false;
}

bool PlagueCenserSkill::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	if( true )
	{
		int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );

		int iDamage = iRankMultiple * m_iSkillRank;
		m_eDamageType = eDT_Viral;
		int iResistStr = m_iMinResist + (m_iSkillRank * m_iResistMod);
		float fRadius = m_fMinRadius + ( m_iSkillRank * m_fRadiusMod );

		kpuVector vTarget = kpuv_Zero;
		g_pGameState->GetLevel()->GetGrid()->GetTileLocation(m_iTargetTile, vTarget);

		//place censer
		g_pGameState->AddActor(new PlagueCenser(vTarget, iDamage, m_eDamageType, iResistStr, fRadius));
		m_bExecuted = true;
		return false;
	}

	return true;
}


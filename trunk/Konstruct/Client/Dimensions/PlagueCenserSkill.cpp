#include "StdAfx.h"
#include "PlagueCenserSkill.h"
#include "PlagueCenser.h"
#include "PlayerCharacter.h"

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

		int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );

		m_fDamage = iRankMultiple * m_iSkillRank;
		m_fRange = MIN_RANGE + (m_iSkillRank * m_fRangeMod);
		m_eDamageType = eDT_Viral;
		m_iResistStr = MIN_RESIST_STR + (m_iSkillRank * m_fResistMod);
		m_fRadius = MIN_RADIUS + ( m_iSkillRank * m_fRadiusMod );
		m_fSpeed = MIN_SPEED - m_iSkillRank;

		pSkillOwner->SetActiveSkill(this);

		return true;
	}

	return false;
}

bool PlagueCenserSkill::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	//need to check for placement of the censer
	//input check required

	kpuVector vTarget;

	m_fElaspedSinceCast += fDeltaTime;

	if( m_fElaspedSinceCast >= m_fSpeed )
	{
		//place censer
		g_pGameState->AddActor(new PlagueCenser(vTarget, m_fDamage, m_eDamageType, m_iResistStr, m_fRadius));
		m_bExecuted = true;
		return false;
	}

	return true;
}

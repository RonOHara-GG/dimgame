#include "StdAfx.h"
#include "FireBall.h"
#include "PlayerCharacter.h"

FireBall::FireBall(void)
{
}

FireBall::~FireBall(void)
{
}

bool FireBall::Activate(PlayerCharacter *pSkillOwner)
{
	if( m_bReady )
	{
		m_fRange = m_fMinRange + m_iSkillRank;
		m_fRadius = m_fMinRadius + (m_iSkillRank / m_fRadiusMod);
		
		int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );
		m_iDamage = iRankMultiple * m_iSkillRank;
		m_iResistStr = m_iMinResist + int(m_iSkillRank * m_fResistMod);

		m_fElaspedSinceCast = 0.0f;
		m_bReady = false;
		m_bExecuted = false;

		pSkillOwner->SetActiveSkill(this);
		return true;
	}

	return false;
}

bool FireBall::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if( m_fElaspedSinceCast >= m_fSpeed )
	{
		//shoot fireball
		Projectile* pArrow = new Projectile(Projectile::ePT_Spell, m_iDamage, m_fRange, m_eDamageType, pSkillOwner, pSkillOwner->GetLocation(), pSkillOwner->GetHeading(), m_fRadius, m_iResistStr);
		g_pGameState->AddActor(pArrow);

		m_bExecuted = true;
		return true;
	}

	return false;
}
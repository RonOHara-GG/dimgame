#include "StdAfx.h"
#include "FireBall.h"
#include "PlayerCharacter.h"

FireBall::FireBall(void)
{
}

FireBall::~FireBall(void)
{
}

bool FireBall::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	//Go on animation
	if( true )
	{
		float fRadius = m_fMinRadius + (m_iSkillRank / m_fRadiusMod);
		
		int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );
		int iDamage = iRankMultiple * m_iSkillRank;
		int iResistStr = m_iMinResist + m_iSkillRank * m_iResistMod;

		//shoot fireball
		Projectile* pArrow = new Projectile(Projectile::ePT_Spell, iDamage, GetRange(), m_eDamageType, pSkillOwner, pSkillOwner->GetLocation(), pSkillOwner->GetHeading(), fRadius, iResistStr);
		g_pGameState->AddActor(pArrow);

		m_bExecuted = true;
		return true;
	}

	return false;
}
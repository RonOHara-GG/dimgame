#include "StdAfx.h"
#include "BurstFire.h"
#include "Weapon.h"
#include "PlayerCharacter.h"

#define MIN_SHOTS 6

BurstFire::BurstFire(void)
{
}

BurstFire::~BurstFire(void)
{
}

bool BurstFire::Activate(PlayerCharacter *pSkillOwner)
{
	if( Skill::Activate(pSkillOwner) )
	{
		m_iShotsToFire = MIN_SHOTS + m_iSkillRank;		
		m_eDamageType = m_pEquipped->GetDamageType();		
		m_fRecovery = m_pEquipped->GetRecovery() * m_fRecoveryMod;

		return true;
	}

	return false;
}

bool BurstFire::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if( m_fElaspedSinceCast >= m_fSpeed)
	{
		m_fElaspedSinceCast = 0.0f;		

		int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );
		int iDamage = int(m_pEquipped->GetDamage() * m_fDamageMod) + (iRankMultiple * m_iSkillRank);

		Projectile* pBullet = new Projectile(Projectile::ePT_Bullet, iDamage, m_pEquipped->GetRange(), m_eDamageType, pSkillOwner, pSkillOwner->GetLocation(), pSkillOwner->GetHeading());
		g_pGameState->AddActor(pBullet);

		m_iShotsFired++;
	}

	if(m_iShotsFired >= m_iShotsToFire)
	{		
		return false;
	}

	return true;


}

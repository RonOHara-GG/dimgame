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
	if( m_bReady )
	{
		m_fElaspedSinceCast = 0;
		m_iShotsFired = 0;
		m_bReady = false;
		m_bExecuted = false;
			
		int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );
		Weapon* pEquipped = pSkillOwner->GetEquippedWeapon();
		m_fRange = pEquipped->GetRange();
		m_iShotsToFire = MIN_SHOTS + m_iSkillRank;
		m_fDamage = (pEquipped->GetDamage() * m_fDamageMod) + (iRankMultiple * m_iSkillRank);
		m_eDamageType = pEquipped->GetDamageType();
		m_fSpeed = pEquipped->GetSpeed() * m_fSpeedMod;
		m_fRecovery = pEquipped->GetRecovery() * m_fRecoveryMod;

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

		Projectile* pBullet = new Projectile(Projectile::ePT_Bullet, m_fDamage, m_fRange, m_eDamageType, pSkillOwner, pSkillOwner->GetLocation(), pSkillOwner->GetHeading());
		g_pGameState->AddActor(pBullet);

		m_iShotsFired++;
	}

	if(m_iShotsFired >= m_iShotsToFire)
	{		
		return false;
	}

	return true;


}

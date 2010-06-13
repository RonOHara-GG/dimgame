#include "StdAfx.h"
#include "AreaSpray.h"
#include "Weapon.h"
#include "PlayerCharacter.h"
#include "Common/Utility/kpuMatrix.h"

#define ARC_ANGLE 1.5707963267948966192313216916398
#define MIN_SHOTS 3

AreaSpray::AreaSpray(void)
{
}

AreaSpray::~AreaSpray(void)
{
}

bool AreaSpray::Activate(PlayerCharacter *pSkillOwner)
{
	if( m_bReady )
	{
		m_fElaspedSinceCast = 0;
		m_iShotsFired = 0;
		m_bReady = false;
		m_bExecuted = false;
			
		int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );
		Weapon* pEquipped = pSkillOwner->GetEquippedWeapon();
		m_fRange = pEquipped->GetRange() * m_fRangeMod;
		m_iShotsToFire = MIN_SHOTS + m_iSkillRank / m_iShotsMod;
		m_fDamage = (pEquipped->GetDamage() * m_fDamageMod) + (iRankMultiple * m_iSkillRank);
		m_eDamageType = pEquipped->GetDamageType();
		m_fSpeed = pEquipped->GetSpeed() * m_fSpeedMod;
		m_fRecovery = pEquipped->GetRecovery() * m_fRecoveryMod;
	
		m_fAngleFreq = ARC_ANGLE / m_iShotsToFire;
		m_fPlayerSpeed = pSkillOwner->GetSpeed();
		pSkillOwner->SetSpeed(0.0f);

	}

	return false;

}

bool AreaSpray::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if( m_fElaspedSinceCast >= m_fSpeed)
	{
		m_fElaspedSinceCast = 0.0f;
		kpuVector vDir = pSkillOwner->GetHeading() % kpuv_OneY;

		kpuMatrix rotMatrix;
		rotMatrix.Identity();

		rotMatrix.SetRotationY( (m_iShotsFired + 1) * m_fAngleFreq);

		Projectile* pBullet = new Projectile(Projectile::ePT_Bullet, m_fDamage, m_fRange, m_eDamageType, pSkillOwner, pSkillOwner->GetLocation(), vDir * rotMatrix);
		g_pGameState->AddActor(pBullet);

		m_iShotsFired++;
	}

	if(m_iShotsFired >= m_iShotsToFire)
	{
		pSkillOwner->SetSpeed(m_fPlayerSpeed);
		return false;
	}

	return true;
}

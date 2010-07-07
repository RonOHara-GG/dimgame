#include "StdAfx.h"
#include "AreaSpray.h"
#include "Weapon.h"
#include "PlayerCharacter.h"
#include "Common/Utility/kpuMatrix.h"

#define ARC_ANGLE 1.5707963267948966192313216916398f
#define MIN_SHOTS 3

AreaSpray::AreaSpray(void)
{
}

AreaSpray::~AreaSpray(void)
{
}

int	AreaSpray::GetRange() { return m_pEquipped->GetRange() * m_fRangeMod; }
float AreaSpray::GetSpeed() { return m_pEquipped->GetSpeed() * m_fSpeedMod; }

bool AreaSpray::Activate(PlayerCharacter *pSkillOwner)
{
	if( Skill::Activate(pSkillOwner) )
	{
		
		m_iShotsToFire = MIN_SHOTS + m_iSkillRank / m_iShotsMod;
		
		m_eDamageType = m_pEquipped->GetDamageType();
		m_fRecovery = m_pEquipped->GetRecovery() * m_fRecoveryMod;
	
		m_fAngleFreq = ARC_ANGLE / m_iShotsToFire;
		m_fPlayerSpeed = pSkillOwner->GetSpeed();
		pSkillOwner->SetSpeed(0.0f);

	}

	return false;

}

bool AreaSpray::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if( m_fElaspedSinceCast >= GetSpeed())
	{
		m_fElaspedSinceCast = 0.0f;
		kpuVector vDir = pSkillOwner->GetHeading() % kpuv_OneY;

		kpuMatrix rotMatrix;
		rotMatrix.Identity();

		rotMatrix.SetRotationY( (m_iShotsFired + 1) * m_fAngleFreq);

		int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );
		int iDamage = int(m_pEquipped->GetDamage() * m_fDamageMod) + (iRankMultiple * m_iSkillRank);

		Projectile* pBullet = new Projectile(Projectile::ePT_Bullet, m_iDamage, GetRange(), m_eDamageType, pSkillOwner, pSkillOwner->GetLocation(), vDir * rotMatrix);
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

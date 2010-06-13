#include "StdAfx.h"
#include "ExplosiveShot.h"
#include "ExplosiveProjectile.h"
#include "Weapon.h"
#include "PlayerCharacter.h"

ExplosiveShot::ExplosiveShot(void)
{
}

ExplosiveShot::~ExplosiveShot(void)
{
}

bool ExplosiveShot::Activate(PlayerCharacter* pSkillOwner)
{
	if(m_bReady)
	{
		Weapon* pEquippedWeapon = pSkillOwner->GetEquippedWeapon();

		m_fElaspedSinceCast = 0.0f;

		int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );

		m_fDamage = pEquippedWeapon->GetDamage();
		m_fRange = pEquippedWeapon->GetRange();
		m_fSpeed = pEquippedWeapon->GetSpeed() * m_fSpeedMod;
		m_fRecovery = pEquippedWeapon->GetRecovery() * m_fRecoveryMod;
		m_eDamageType = pEquippedWeapon->GetDamageType();
		m_eRadiusType = eDT_Heat;
		m_fRadiusDamage = iRankMultiple * m_iSkillRank;
		m_iRadius = m_iSkillRank / m_iRadiusMod;
		

		pSkillOwner->SetActiveSkill(this);
		
		m_bReady = false;
		m_bExecuted = false;		
		
		return true;		
	}

	return false;
}

bool ExplosiveShot::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if( !m_bExecuted && m_fElaspedSinceCast >= m_fSpeed * 0.5f )
	{
		//explosive arrow
		Projectile* pArrow = new ExplosiveProjectile(Projectile::ePT_Arrow, m_fDamage, m_fRange, m_eDamageType, pSkillOwner, pSkillOwner->GetLocation(), pSkillOwner->GetHeading(),
			m_iRadius, m_fRadiusDamage, m_eRadiusType);

		g_pGameState->AddActor(pArrow);
		m_bExecuted = true;
	}

	if( m_fElaspedSinceCast >= m_fSpeed )
		return false;

	return true;


}

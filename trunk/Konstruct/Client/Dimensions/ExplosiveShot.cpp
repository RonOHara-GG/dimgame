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

bool ExplosiveShot::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	//fire when animation is ready
	if( true )
	{
		Weapon* pEquippedWeapon = pSkillOwner->GetEquippedWeapon();

		int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );

		int iDamage = pEquippedWeapon->GetDamage();
		float fRange = pEquippedWeapon->GetRange();
		m_fRecovery = pEquippedWeapon->GetRecovery() * m_fRecoveryMod;		
		int iRadiusDamage = iRankMultiple * m_iSkillRank;
		float fRadius = m_iSkillRank * m_fRadiusMod;

		//explosive arrow
		Projectile* pArrow = new ExplosiveProjectile(Projectile::ePT_Arrow, iDamage, fRange, pEquippedWeapon->GetDamageType(), pSkillOwner, pSkillOwner->GetLocation(), pSkillOwner->GetHeading(),
			fRadius, iRadiusDamage, eDT_Heat);

		g_pGameState->AddActor(pArrow);
		m_bExecuted = true;
		return false;
	}


	return true;


}

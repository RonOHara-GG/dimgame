#include "StdAfx.h"
#include "BasicShot.h"
#include "Weapon.h"
#include "PlayerCharacter.h"


BasicShot::BasicShot(void)
{
}

BasicShot::~BasicShot(void)
{
}

bool BasicShot::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{	
	//Fire according to animation
	if( true )
	{
		Weapon* pEquippedWeapon = pSkillOwner->GetEquippedWeapon();

		int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );

		int iDamage = pEquippedWeapon->GetDamage() + (iRankMultiple * m_iSkillRank);
		float fRange = pEquippedWeapon->GetRange() + (m_iSkillRank * m_fRangeMultiple);

		//fire arrow
		Projectile* pArrow = new Projectile(Projectile::ePT_Arrow, iDamage, fRange, pEquippedWeapon->GetDamageType(), pSkillOwner, pSkillOwner->GetLocation(), pSkillOwner->GetHeading());
		g_pGameState->AddActor(pArrow);
		m_bExecuted = true;

		return false;
	}

	return true;	
}
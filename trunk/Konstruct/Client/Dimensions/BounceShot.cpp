#include "StdAfx.h"
#include "BounceShot.h"
#include "Weapon.h"
#include "PlayerCharacter.h"
#include "BounceArrow.h"

#define MIN_BOUNCE_RANGE 2

BounceShot::BounceShot(void)
{
}

BounceShot::~BounceShot(void)
{
}

bool BounceShot::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	//Fire when animation is ready
	if( true )
	{
		float fRange = pSkillOwner->GetEquippedWeapon()->GetRange();
		int iDamage = pSkillOwner->GetEquippedWeapon()->GetDamage() * (m_iDamageMultiple * ( m_iDamageMultiple * m_iSkillRank * m_iRankMultipleMin ));
		float fBounceRange = MIN_BOUNCE_RANGE + ( m_iSkillRank * m_fBounceModifier );

		//fire arrow
		BounceArrow* pArrow = new BounceArrow(iDamage, fRange, pSkillOwner->GetEquippedWeapon()->GetDamageType(), pSkillOwner, pSkillOwner->GetLocation(), pSkillOwner->GetHeading(), fBounceRange);
		g_pGameState->AddActor(pArrow);
		m_bExecuted = true;

		return false;
	}

	return true;	
}
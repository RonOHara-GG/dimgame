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

bool BounceShot::Activate(PlayerCharacter *pSkillOwner)
{
	if( m_bReady )
	{
		m_fRange = pSkillOwner->GetEquippedWeapon()->GetRange();
		m_fDamage = pSkillOwner->GetEquippedWeapon()->GetDamage() * (m_fDamageMultiple * ( m_fDamageMultiple * m_iSkillRank * m_iRankMultipleMin ));
		m_fBounceRange = MIN_BOUNCE_RANGE + ( m_iSkillRank * m_fBounceModifier );

		m_bReady = false;
		m_bExecuted = false;
		m_fElaspedSinceCast = 0.0f;		

		pSkillOwner->SetActiveSkill(this);
		
		return true;
	}

	return false;
}

bool BounceShot::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if( !m_bExecuted && m_fElaspedSinceCast >= m_fSpeed * 0.5f )
	{
		//fire arrow
		BounceArrow* pArrow = new BounceArrow(m_fDamage, m_fRange, m_eDamageType, pSkillOwner, pSkillOwner->GetLocation(), pSkillOwner->GetHeading(), m_fBounceRange);
		g_pGameState->AddActor(pArrow);
		m_bExecuted = true;
	}

	if( m_fElaspedSinceCast >= m_fSpeed )
		return false;

	return true;	
}
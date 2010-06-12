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

bool BasicShot::Activate(PlayerCharacter *pSkillOwner)
{	
	if(m_bReady)
	{
		Weapon* pEquippedWeapon = pSkillOwner->GetEquippedWeapon();

		m_fElaspedSinceCast = 0.0f;

		int iRankMultiple = m_fRankMultipleMin + ( rand() % (int)(m_fRankMultipleMax - m_fRankMultipleMin) );

		m_fDamage = pEquippedWeapon->GetDamage() + (iRankMultiple * m_iSkillRank);
		m_fRange = pEquippedWeapon->GetRange() + (m_iSkillRank * m_fRangeMultiple);
		m_fSpeed = pEquippedWeapon->GetSpeed();
		m_eDamageType = pEquippedWeapon->GetDamageType();

		pSkillOwner->SetActiveSkill(this);
		
		m_bReady = false;
		m_bExecuted = false;		
		
		return true;		
	}

	return false;
}

bool BasicShot::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if( !m_bExecuted && m_fElaspedSinceCast >= m_fSpeed * 0.5f )
	{
		//fire arrow
		Projectile* pArrow = new Projectile(Projectile::ePT_Arrow, m_fDamage, m_fRange, m_eDamageType, pSkillOwner, pSkillOwner->GetLocation(), pSkillOwner->GetHeading());
		g_pGameState->AddActor(pArrow);
		m_bExecuted = true;
	}

	if( m_fElaspedSinceCast >= m_fSpeed )
		return false;

	return true;	
}
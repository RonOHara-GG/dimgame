#include "StdAfx.h"
#include "Launch.h"
#include "Weapon.h"
#include "PlayerCharacter.h"
#include "Projectile.h"

Launch::Launch(void)
{
}

Launch::~Launch(void)
{
}

bool Launch::Activate(PlayerCharacter *pSkillOwner)
{
	if( m_bReady )
	{
		Weapon* pEquipped = pSkillOwner->GetEquippedWeapon();
		int iRankMultiple = m_iRankMultipleMin + ( rand() % (int)(m_iRankMultipleMax - m_iRankMultipleMin) );

	    m_iRange = pEquipped->GetRange() + (m_iSkillRank / m_iRangeMod);
		m_fDamage = pEquipped->GetDamage() + (iRankMultiple * m_iSkillRank);
		m_fSpeed = pEquipped->GetSpeed();
		m_fRecovery = pEquipped->GetRecovery();

		m_bExecuted = false;
		m_bReady = false;
		m_fElaspedSinceCast = 0.0f;
		m_pTarget = 0;

		pSkillOwner->SetActiveSkill(this);
	}

	return false;
}

bool Launch::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	//Get target from input

	

	if( m_pTarget )
	{
		m_fElaspedSinceCast += fDeltaTime;

		if( m_fElaspedSinceCast >= m_fSpeed )
		{
			//send projectile at target
			kpuVector vDir = m_pTarget->GetLocation() - m_vSource;
			vDir.Normalize();

			Projectile* pMissile = new Projectile(Projectile::ePT_Missile, m_fDamage, m_iRange, m_eDamageType, pSkillOwner, m_vSource, vDir);
			g_pGameState->AddActor(pMissile);
			return false;
		}

	}

	return true;
}
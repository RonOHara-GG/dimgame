#include "StdAfx.h"
#include "RapidShot.h"
#include "Weapon.h"
#include "PlayerCharacter.h"
#include "Grid.h"
#include "Level.h"
#include "Common/utility/kpuQuadTree.h"

#define MIN_SHOT_COUNT 4

RapidShot::RapidShot(void)
{
}

RapidShot::~RapidShot(void)
{
}

bool RapidShot::Activate(PlayerCharacter *pSkillOwner)
{
	if( m_bReady )
	{
		m_fDamage = pSkillOwner->GetEquippedWeapon()->GetDamage() * (m_fDamageMultiple + (m_iRankMultipleMax * m_iSkillRank));
		m_fRange = pSkillOwner->GetEquippedWeapon()->GetRange() * m_fRangeMultiple;
		m_iShotMax = MIN_SHOT_COUNT + (m_iSkillRank * m_fShotMultiple);
		m_fSpeed = pSkillOwner->GetEquippedWeapon()->GetSpeed() * m_fSpeedMultiple;

		m_bReady = false;
		m_bExecuted = false;		
		
		m_iShotCount = 0;

		//save the players speed and set his current to 0
		m_fPlayersSpeed = pSkillOwner->GetSpeed();

		pSkillOwner->SetSpeed(0.0f);

		pSkillOwner->SetActiveSkill(this);

		return true;
	}

	return false;
}

bool RapidShot::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	//see if it is time to shoot another arrow
	if( m_fElaspedSinceCast >= m_fSpeed * 0.5f )
	{
		m_fElaspedSinceCast = 0.0f;

		//fire new arrow
		m_iShotCount++;
		Projectile* pArrow = new Projectile(Projectile::ePT_Arrow, m_fDamage, m_fRange, m_eDamageType, pSkillOwner, pSkillOwner->GetLocation(), pSkillOwner->GetHeading());
		g_pGameState->AddActor(pArrow);

		if( m_iShotCount == m_iShotMax )
		{
			pSkillOwner->SetSpeed(m_fPlayersSpeed);
			return false;
		}
	}
	
}

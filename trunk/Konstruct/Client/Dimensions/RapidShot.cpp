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

int RapidShot::GetRange() { return  m_pEquipped->GetRange() * m_fRangeMultiple; }
float RapidShot::GetSpeed() { return m_pEquipped->GetSpeed() * m_fSpeedMod; }

bool RapidShot::Activate(PlayerCharacter *pSkillOwner)
{
	if( Skill::Activate(pSkillOwner) )
	{		
		m_iShotMax = MIN_SHOT_COUNT + (m_iSkillRank / m_iShotMultiple);		
		
		m_iShotCount = 0;

		//save the players speed and set his current to 0
		m_fPlayersSpeed = pSkillOwner->GetSpeed();

		pSkillOwner->SetSpeed(0.0f);

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
		int iDamage = pSkillOwner->GetEquippedWeapon()->GetDamage() * (m_iDamageMultiple + (m_iRankMultipleMax * m_iSkillRank));

		//fire new arrow
		m_iShotCount++;
		Projectile* pArrow = new Projectile(Projectile::ePT_Arrow, iDamage, GetRange(), m_eDamageType, pSkillOwner, pSkillOwner->GetLocation(), pSkillOwner->GetHeading());
		g_pGameState->AddActor(pArrow);

		if( m_iShotCount == m_iShotMax )
		{
			pSkillOwner->SetSpeed(m_fPlayersSpeed);
			return false;
		}
	}

	return true;
	
}

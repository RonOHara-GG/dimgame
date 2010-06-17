#include "StdAfx.h"
#include "MultiShot.h"
#include "Weapon.h"
#include "PlayerCharacter.h"
#include "Grid.h"
#include "Level.h"
#include "Common/Utility/kpuQuadTree.h"
#include "Common/Utility/kpuMatrix.h"

MultiShot::MultiShot(void)
{
}

MultiShot::~MultiShot(void)
{
}

bool MultiShot::Activate(PlayerCharacter *pSkillOwner)
{
	if(m_bReady)
	{
		Weapon* pEquippedWeapon = pSkillOwner->GetEquippedWeapon();

		m_fElaspedSinceCast = 0.0f;		

		m_iShotsToFire = MIN_SHOTS_FIRED + ( m_iSkillRank / m_iNumOfShotsMultiple );
		m_iDamage = pEquippedWeapon->GetDamage() + (m_iShotsToFire - (pSkillOwner->GetInt() * m_fIntMultiple));
		m_fRange = pEquippedWeapon->GetRange();
		m_eDamageType = pEquippedWeapon->GetDamageType();
		m_fSpeed = pEquippedWeapon->GetSpeed() * SPEED_MULTIPLE;

		m_fFireArc = m_fFireArcMultiple * m_iSkillRank;

		pSkillOwner->SetActiveSkill(this);
		
		m_bReady = false;
		m_bExecuted = false;

		pSkillOwner->SetActiveSkill(this);

		return true;		
	}

	return false;


}

bool MultiShot::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if( !m_bExecuted && m_fElaspedSinceCast >= m_fSpeed * 0.5f )
	{
		//shoot
		float fRadiansPerShot = m_fFireArc / m_iShotsToFire;
		fRadiansPerShot *= DEG_TO_RAD;
		kpuVector vDir = pSkillOwner->GetHeading() % kpuv_OneY;
		
		for(int i = 0; i < m_iShotsToFire; i++ )
		{
			kpuMatrix rotMatrix;
			rotMatrix.Identity();

			rotMatrix.SetRotationY( (i + 1) * fRadiansPerShot);

			Projectile* pArrow = new Projectile(Projectile::ePT_Arrow, m_iDamage, m_fRange, m_eDamageType, pSkillOwner, pSkillOwner->GetLocation(), vDir * rotMatrix);
			g_pGameState->AddActor(pArrow);
		}

		m_bExecuted = true;
		
	}

	if( m_fElaspedSinceCast >= m_fSpeed )
		return false;

	return true;
}

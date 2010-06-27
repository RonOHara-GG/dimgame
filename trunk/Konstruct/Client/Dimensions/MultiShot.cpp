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

bool MultiShot::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	//fire when animation is ready
	if( true )
	{
		Weapon* pEquippedWeapon = pSkillOwner->GetEquippedWeapon();	

		int iShotsToFire = MIN_SHOTS_FIRED + ( m_iSkillRank / m_iNumOfShotsMultiple );
		int iDamage = pEquippedWeapon->GetDamage() + (iShotsToFire - int(pSkillOwner->GetInt() * m_fIntMultiple));
		float fRange = pEquippedWeapon->GetRange();
		float fFireArc = m_fFireArcMultiple * m_iSkillRank;

		//shoot
		float fRadiansPerShot = fFireArc / iShotsToFire;
		fRadiansPerShot *= DEG_TO_RAD;
		kpuVector vDir = pSkillOwner->GetHeading() % kpuv_OneY;
		
		for(int i = 0; i < iShotsToFire; i++ )
		{
			kpuMatrix rotMatrix;
			rotMatrix.Identity();

			rotMatrix.SetRotationY( (i + 1) * fRadiansPerShot);

			Projectile* pArrow = new Projectile(Projectile::ePT_Arrow, iDamage, fRange, pEquippedWeapon->GetDamageType(), pSkillOwner, pSkillOwner->GetLocation(), vDir * rotMatrix);
			g_pGameState->AddActor(pArrow);
		}

		m_bExecuted = true;
		
	}

	if( m_fElaspedSinceCast >= m_fSpeed )
		return false;

	return true;
}

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

int Launch::GetRange(Actor* p1)
{
	if( !m_pEquipped )
		return 0;

	return m_pEquipped->GetRange() + (m_iSkillRank / m_iRangeMod);
}

bool Launch::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	//Get target from input	

	if( m_pTarget )
	{
		m_fElaspedSinceCast += fDeltaTime;

		if( m_fElaspedSinceCast >= m_fSpeed )
		{
			int iRankMultiple = m_iRankMultipleMin + ( rand() % (m_iRankMultipleMax - m_iRankMultipleMin) );

			int iRange = GetRange(0);
			int iDamage = m_pEquipped->GetDamage() + (iRankMultiple * m_iSkillRank);
			
			m_fRecovery = m_pEquipped->GetRecovery();

			//send projectile at target
			kpuVector vDir = m_pTarget->GetLocation() - m_vSource;
			vDir.Normalize();

			Projectile* pMissile = new Projectile(Projectile::ePT_Missile, iDamage, iRange, m_eDamageType, pSkillOwner, m_vSource, vDir);
			g_pGameState->AddActor(pMissile);
			return false;
		}

	}

	return true;
}
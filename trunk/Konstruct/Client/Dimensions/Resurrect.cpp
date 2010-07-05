#include "StdAfx.h"
#include "Resurrect.h"
#include "PlayerCharacter.h"

#define MIN_HEAL 10

Resurrect::Resurrect(void)
{
}

Resurrect::~Resurrect(void)
{
}

bool Resurrect::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	//perform on animation
	if(true)
	{
		if( !m_pTarget )
		{
			if( m_pTarget->IsAlive() )
			{
				//cancel skill
				return false;
			}
			
			int iHeal = MIN_HEAL + (m_iSkillRank * m_iHealMod);

			m_pTarget->Heal(m_pTarget->GetMaxHealth() * iHeal);
			m_pTarget->HealMental(m_pTarget->GetMaxMental() * iHeal);

			m_bExecuted = true;
			return false;
		}
	}

	return true;
}

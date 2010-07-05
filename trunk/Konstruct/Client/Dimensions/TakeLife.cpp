#include "StdAfx.h"
#include "TakeLife.h"
#include "PlayerCharacter.h"

TakeLife::TakeLife(void)
{
}

TakeLife::~TakeLife(void)
{
}

bool TakeLife::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	if( m_pTarget )
	{
		//Cast when animation is ready for it		
		if( true )
		{				
			int iResistStr = m_iMinResist + int(m_iSkillRank * m_fResistMod);		

			m_pTarget->TakeDamage(m_pTarget->GetCurrentHealth(), m_eDamageType, iResistStr);

			m_bExecuted = true;
			return false;
		}
	}	

	return true;
}
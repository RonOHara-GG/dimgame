#include "StdAfx.h"
#include "SummonPet.h"
#include "PlayerCharacter.h"
#include "PlayerPet.h"

SummonPet::SummonPet(void)
{
}

SummonPet::~SummonPet(void)
{
}



bool SummonPet::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if( m_fElaspedSinceCast >= m_fSpeed )
	{
		pSkillOwner->AddPet(new PlayerPet(pSkillOwner, m_iSkillRank));
		m_bExecuted = true;
		return true;
	}
	return false;
}
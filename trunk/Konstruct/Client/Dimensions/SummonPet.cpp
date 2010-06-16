#include "StdAfx.h"
#include "SummonPet.h"
#include "PlayerCharacter.h"
#include "PlayerPet.h"

#define MIN_RANGE 1

SummonPet::SummonPet(void)
{
}

SummonPet::~SummonPet(void)
{
}

bool SummonPet::Activate(PlayerCharacter *pSkillOwner)
{
	if( m_bReady )
	{
		m_fElaspedSinceCast = 0.0f;
		m_bReady = false;
		m_bExecuted = false;

		m_iRange = MIN_RANGE + (m_iSkillRank / m_iRangeMod);
		pSkillOwner->SetActiveSkill(this);

		return true;
	}

	return false;
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
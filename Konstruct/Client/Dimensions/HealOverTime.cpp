#include "StdAfx.h"
#include "HealOverTime.h"
#include "PlayerCharacter.h"

HealOverTime::HealOverTime(int iHeal, float fDuration)
{
	m_iHeal = iHeal;
	m_fDuration = fDuration;
	m_fElapsedTotal = 0.0f;
	m_fElapsedTick = 0.0f;
}

HealOverTime::~HealOverTime(void)
{
}

bool HealOverTime::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	m_fElapsedTotal += fDeltaTime;
	m_fElapsedTick += fDeltaTime;

	if( m_fElapsedTick >= 1.0f )
	{
		pSkillOwner->Heal(m_iHeal);
		m_fElapsedTick = 0.0f;
	}

	if( m_fElapsedTotal >= m_fDuration )
		return false;

	return true;
}
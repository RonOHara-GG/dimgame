#include "StdAfx.h"
#include "TakeLife.h"
#include "PlayerCharacter.h"

TakeLife::TakeLife(void)
{
}

TakeLife::~TakeLife(void)
{
}

bool TakeLife::Activate(PlayerCharacter* pSkillOwner)
{
	if(m_bReady)
	{
		m_fRange = m_fMinRange + m_iSkillRank /  m_iRangeMod;		
		m_fResistStr = m_fMinResist + (m_iSkillRank * m_fResistMod);		

		pSkillOwner->SetActiveSkill(this);
		
		m_bReady = false;
		m_bExecuted = false;	
		m_pTarget = 0;
		
		return true;		
	}

	return false;
}

bool TakeLife::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	//check input and see if mouse was clicked

	//if left mouse clicked then target selected and get the target	

	//if right mouse clicked then cancel skill
	//return false;

	if( m_pTarget )
	{
		m_fElaspedSinceCast += fDeltaTime;		

		if( m_fElaspedSinceCast >= m_fSpeed )
		{
			if( m_pTarget->GetResist(m_eDamageType) < m_fResistStr )
				m_pTarget->TakeDamage(m_pTarget->GetCurrentHealth(), m_eDamageType, m_fResistStr);
			m_bExecuted = true;
			return false;
		}
	}	

	return true;
}
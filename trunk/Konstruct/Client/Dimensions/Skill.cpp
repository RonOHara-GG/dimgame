#include "StdAfx.h"
#include "Skill.h"
#include "Actor.h"
#include "PlayerCharacter.h"

Skill::Skill(void)
{
	m_iSkillRank = 1;
	m_iRequiredLevel = 0;
	memset(m_szName, 0, sizeof(m_szName));
	m_fElaspedSinceCast = 0;
	m_fRecovery = 0;
	m_fElaspedRecovery = 0;
	m_bReady = true;
}

Skill::~Skill(void)
{
}



void Skill::UpdateTimers(float fGameTime)
{
	if(!m_bReady)
	{
		m_fElaspedRecovery += fGameTime;

		if(m_fElaspedRecovery >= m_fRecovery)
		{
			m_bReady = true;
			m_fElaspedRecovery = 0.0f;
		}
	}
}

//bool Skill::FinishedCasting(float fGameTime)
//{
//	m_fElaspedSinceCast += fGameTime;
//
//	if(m_fElaspedSinceCast >= m_fSpeed)
//	{
//		m_fElaspedSinceCast = 0.0f;
//		return true;
//	}
//
//	return false;
//}


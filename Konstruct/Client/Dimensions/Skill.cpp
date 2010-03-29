#include "StdAfx.h"
#include "Skill.h"
#include "Actor.h"
#include "PlayerCharacter.h"

Skill::Skill(void)
{
	m_iRequiredLevel = -1;
	m_iRange = -1;
	m_iRadius = -1;
	m_iDamage = -1;
	m_eDamageType = eDT_Crushing;
	m_fSpeed = -1;
	m_fRecovery = -1;
	m_fElaspedRecovery = -1;
	m_bReadyToUse = false;
}

Skill::~Skill(void)
{
}

bool Skill::Use(Actor *pTarget, PlayerCharacter *pSkillOwner, Grid *pGrid)
{
	if(m_bReadyToUse)
	{
		//Check if target is attack able
		if(pTarget->Attackable())
		{
			if(pSkillOwner->IsInRange(pTarget, m_iRange, pGrid))
			{
				m_bReadyToUse = false;
				return true;
			}
		}
	}

	return false;
}

void Skill::UpdateTimers(float fGameTime)
{
	if(!m_bReadyToUse)
	{
		m_fElaspedRecovery += fGameTime;

		if(m_fElaspedRecovery >= m_fRecovery)
		{
			m_bReadyToUse = true;
			m_fElaspedRecovery = 0.0f;
		}
	}
}
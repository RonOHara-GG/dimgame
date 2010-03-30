#include "StdAfx.h"
#include "Skill.h"
#include "Actor.h"
#include "PlayerCharacter.h"

Skill::Skill(void)
{
	m_iRequiredLevel = 0;
	m_iRange = 0;
	m_iRadius = 0;
	m_iDamage = 0;
	m_eDamageType = eDT_Crushing;
	m_fSpeed = 0;
	m_fElaspedSinceCast = 0;
	m_fRecovery = 0;
	m_fElaspedRecovery = 0;
	m_bReady = false;
}

Skill::~Skill(void)
{
}

bool Skill::Use(Actor *pTarget, PlayerCharacter *pSkillOwner, Grid *pGrid)
{
	if(m_bReady)
	{
		//Check if target is attack able
		if(pTarget->Attackable())
		{
			if(pSkillOwner->IsInRange(pTarget, m_iRange, pGrid))
			{
				pSkillOwner->SetActiveSkill(this);
				m_bReady = false;
				return true;
			}
		}
	}

	return false;
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

bool Skill::FinishedCasting(float fGameTime)
{
	m_fElaspedSinceCast += fGameTime;

	if(m_fElaspedSinceCast >= m_fSpeed)
	{
		m_fElaspedSinceCast = 0.0f;
		return true;
	}

	return false;
}

void Skill::ApplyEffect(Actor* pTarget)
{
	if(pTarget)
	{
		pTarget->TakeDamage(m_iDamage, m_eDamageType);
	}
}
#include "StdAfx.h"
#include "Infection.h"
#include "PlayerCharacter.h"

Infection::Infection(void)
{
}

Infection::Infection(Actor* pTarget, float fDamage, float fResist, int iInfectRadius, DamageType eDamageType)
	:PersistentSkill(pTarget)
{
	m_fDamage = fDamage;	
	m_fResistStr = fResist;
	m_iInfectRadius = iInfectRadius;
	m_eDamageType = eDamageType;

	m_pTarget->SetResist(m_pTarget->GetResist(m_eDamageType) - m_fResistStr, m_eDamageType);
}

Infection::~Infection(void)
{
	m_pTarget->SetResist(m_pTarget->GetResist(m_eDamageType) + m_fResistStr, m_eDamageType);
}

bool Infection::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	//deal damage
	if( !m_pTarget->TakeDamage(m_fDamage, m_eDamageType) )
	{
		//resisted so terminate the skill
		m_pTarget->RemovePersistentSkill(this);
		return false;
	}

	return true;
}

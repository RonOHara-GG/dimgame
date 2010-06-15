#include "StdAfx.h"
#include "BlessBuff.h"
#include "Actor.h"

BlessBuff::BlessBuff(Actor* pTarget, float fDamage, float fReduction, float fDuration):
	PersistentSkill(pTarget)
{
	m_fDamageBonus = fDamage;
	m_fDamageReduction = fReduction;
	m_fSpeed = fDuration;

	m_fElaspedSinceCast = 0.0f;

	pTarget->DamageBuff(fDamage);
	pTarget->DamageReduction(fReduction);
}

BlessBuff::~BlessBuff(void)
{
	m_pTarget->DamageBuff(-m_fDamageBonus);
	m_pTarget->DamageReduction(-m_fDamageReduction);
}

bool BlessBuff::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if( m_fElaspedSinceCast >= m_fSpeed )
		return false;

	return true;
}


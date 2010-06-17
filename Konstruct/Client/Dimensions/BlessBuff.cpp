#include "StdAfx.h"
#include "BlessBuff.h"
#include "Actor.h"

BlessBuff::BlessBuff(Actor* pTarget, int iDamage, int iReduction, float fDuration):
	PersistentSkill(pTarget)
{
	m_iDamageBonus = iDamage;
	m_iDamageReduction = iReduction;
	m_fSpeed = fDuration;

	m_fElaspedSinceCast = 0.0f;

	pTarget->DamageBuff(iDamage);
	pTarget->DamageReduction(iReduction);
}

BlessBuff::~BlessBuff(void)
{
	m_pTarget->DamageBuff(-m_iDamageBonus);
	m_pTarget->DamageReduction(-m_iDamageReduction);
}

bool BlessBuff::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	if( m_fElaspedSinceCast >= m_fSpeed )
		return false;

	return true;
}


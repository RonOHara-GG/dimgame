#include "StdAfx.h"
#include "Weapon.h"
#include "Actor.h"
#include "WeaponSkill.h"

Weapon::Weapon(void)
{
	m_fRange = 0.0f;
	m_fDamage = 0.0f;
	m_fRecovery = 0.0f;
	m_fElaspedRecov = 0.0f;
	m_eDamageType = (DamageType)0;
	m_bReady = true;	
}

Weapon::~Weapon(void)
{
}

void Weapon::Update(float fGameTime)
{
	if(!m_bReady)
	{
		m_fElaspedRecov += fGameTime;

		if(m_fElaspedRecov >= GetRecovery())
		{
			m_bReady = true;
			m_fElaspedRecov = 0.0f;
		}		

	}
}

void Weapon::Use(Actor* pTarget)
{
	m_bReady = false;
	pTarget->TakeDamage(GetDamage(), m_eDamageType);
}

float Weapon::GetDamage()
{
	if( m_pWeaponSkill )
		return m_fDamage + m_pWeaponSkill->GetDamageBonus();

	return m_fDamage;
}

float Weapon::GetRange()
{
	if( m_pWeaponSkill )	
		return m_fRange + m_pWeaponSkill->GetRangeBonus();

	return m_fRange;
}

float Weapon::GetRecovery()
{
	if( m_pWeaponSkill )
		return m_fRecovery - m_pWeaponSkill->GetRecoveryBonus();

	return m_fRecovery;
}

float Weapon::GetSpeed()
{
	if( m_pWeaponSkill )
		return m_fSpeed - m_pWeaponSkill->GetSpeedBonus();

	return m_fSpeed;
}
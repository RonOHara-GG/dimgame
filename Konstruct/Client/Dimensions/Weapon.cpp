#include "StdAfx.h"
#include "Weapon.h"
#include "Actor.h"

Weapon::Weapon(void)
{
	m_fRange = 0.0f;
	m_fDamage = 0.0f;
	m_fRecovery = 0.0f;
	m_fElaspedRecov = 0.0f;
	m_fSpeed = 0.0f;
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

		if(m_fElaspedRecov >= m_fRecovery)
		{
			m_bReady = true;
			m_fElaspedRecov = 0.0f;
		}		

	}
}

void Weapon::Use(Actor* pTarget)
{
	m_bReady = false;
	pTarget->TakeDamage(m_fDamage, m_eDamageType);
}
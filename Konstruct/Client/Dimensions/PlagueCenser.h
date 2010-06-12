#pragma once
#include "actor.h"

class PlagueCenser :
	public Actor
{
public:
	PlagueCenser(kpuVector vLocation, float fDamage, DamageType eDamageType, int iResistStr, float fRadius);
	~PlagueCenser(void);

	bool Update(float fGameTime);

protected:
	float		m_fDamage;
	DamageType	m_eDamageType;
	int			m_iResistStr;
	float		m_fRadius;
	float		m_fElaspedTick;
	float		m_fTickTime;
};

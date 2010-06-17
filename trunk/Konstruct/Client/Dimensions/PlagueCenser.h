#pragma once
#include "actor.h"

class PlagueCenser :
	public Actor
{
public:
	PlagueCenser(kpuVector vLocation, int iDamage, DamageType eDamageType, int iResistStr, float fRadius);
	~PlagueCenser(void);

	bool Update(float fGameTime);

protected:
	int			m_iDamage;
	DamageType	m_eDamageType;
	int			m_iResistStr;
	float		m_fRadius;
	float		m_fElaspedTick;
	float		m_fTickTime;
};

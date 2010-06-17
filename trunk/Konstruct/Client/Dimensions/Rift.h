#pragma once
#include "playerpet.h"

class Rift :
	public PlayerPet
{
public:
	Rift(PlayerCharacter* pOwner, int iLevel, float fRadius, kpuVector vLocation, float fResist, float fDuration, DamageType eType);
	~Rift(void);

	bool Update(float fGameTime);

protected:
	DamageType	m_eDamageType;
	float		m_fElasped;
	float		m_fDuration;
	float		m_fRadius;
	float		m_fResistStr;
};

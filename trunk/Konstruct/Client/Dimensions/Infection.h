#pragma once
#include "persistentskill.h"

class Infection :
	public PersistentSkill
{
public:
	Infection(void);
	Infection(Actor* pTarget, float fDamage, float fResist, float fInfectRadius, DamageType eDamageType);
	~Infection(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);	

protected:
	float		m_fDamage;	
	float		m_fResistStr;
	float		m_fInfectRadius;
	DamageType  m_eDamageType;

	float		m_fTickTime;
	float		m_fTickElasped;

};

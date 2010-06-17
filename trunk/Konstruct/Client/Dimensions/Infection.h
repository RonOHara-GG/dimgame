#pragma once
#include "persistentskill.h"

class Infection :
	public PersistentSkill
{
public:
	Infection(void);
	Infection(Actor* pTarget, int iDamage, int iResist, float fInfectRadius, DamageType eDamageType);
	~Infection(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);	

protected:
	int			m_iDamage;	
	int			m_iResistStr;
	float		m_fInfectRadius;
	DamageType  m_eDamageType;

	float		m_fTickTime;
	float		m_fTickElasped;

};

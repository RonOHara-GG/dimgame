#pragma once
#include "persistentskill.h"

class Infection :
	public PersistentSkill
{
public:
	Infection(void);
	Infection(Actor* pTarget, float fDamage, int iResist, int iInfectRadius, DamageType eDamageType);
	~Infection(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);	

protected:
	float		m_fDamage;	
	int			m_iResistStr;
	int			m_iInfectRadius;
	DamageType  m_eDamageType;

	float		m_fTickTime;
	float		m_fTickElasped;

};

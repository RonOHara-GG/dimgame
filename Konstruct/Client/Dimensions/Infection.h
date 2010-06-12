#pragma once
#include "persistentskill.h"

class Infection :
	public PersistentSkill
{
public:
	Infection(void);
	Infection(Actor* pTarget, float fDamage, float fResist, int iInfectRadius, DamageType eDamageType);
	~Infection(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);	

protected:
	float		m_fDamage;	
	float		m_fResistStr;
	int			m_iInfectRadius;
	DamageType  m_eDamageType;

};

#pragma once
#include "persistentskill.h"

class AcidRainCloud :
	public PersistentSkill
{
public:
	AcidRainCloud(kpuVector vLoc, int iDamage, DamageType eDT, float fRadius, int iResist);
	~AcidRainCloud(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	float		m_fRadius;
	int			m_iDamage;
	DamageType	m_eDamageType;
	int			m_iResistStr;

	float		m_fTickTime;
	float		m_fTickElasped;

	kpuVector	m_vLocation;
	
};

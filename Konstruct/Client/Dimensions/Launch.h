#pragma once
#include "skill.h"
#include "Common/Utility/kpuVector.h"

class Launch :
	public Skill
{
public:
	Launch(void);
	~Launch(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	kpuVector		m_vSource;
	Actor*			m_pTarget;
	float			m_fDamage;
	DamageType		m_eDamageType;
	int				m_iRange;

	//--Loaded Data--
	float			m_fDamageMod;
	int				m_iRangeMod;
};

#pragma once
#include "skill.h"

class RestoreBody :
	public Skill
{
public:
	RestoreBody(void);
	~RestoreBody(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

	enum HealType
	{
		eHT_Health,
		eHT_Mind
	};

protected:
	float		m_fRadius;
	float		m_fRadiusMod;
	float		m_fMinRadius;
	int			m_iHeal;
	HealType	m_eHealType;

};

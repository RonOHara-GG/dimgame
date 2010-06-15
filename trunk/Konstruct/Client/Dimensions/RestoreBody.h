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
	int			m_iRadius;
	int			m_iRadiusMod;
	float		m_fHeal;
	HealType	m_eHealType;

};

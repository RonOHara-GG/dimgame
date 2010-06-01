#pragma once
#include "skill.h"


class Strike: public Skill
{
public:
	Strike(void);
	virtual ~Strike(void);

	virtual bool Use(PlayerCharacter* pSkillOwner);
	virtual bool ApplyEffect(PlayerCharacter *pSkillOwner, float fDeltaTime);

protected:

	int			m_iRange; //In tiles
	int			m_iRadius;  //In tiles
	int			m_fDamage;
	int			m_iRankMultiple;	
	float		m_fStrMultiple;
	DamageType  m_eDamageType;
};

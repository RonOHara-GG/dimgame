#pragma once
#include "skill.h"
#include "common/utility/kpuvector.h"


class Strike: public Skill
{
public:
	Strike(void);
	virtual ~Strike(void);

	virtual bool Activate(PlayerCharacter* pSkillOwner);
	virtual bool Update(PlayerCharacter *pSkillOwner, float fDeltaTime);
	void MoveTarget(Actor* pTarget, kpuVector vDirection, float fSpeed); //Move the target in a direction, if the target location is occupied move it's occupant and so on.

protected:

	int			m_iRange; //In tiles
	int			m_iRadius;  //In tiles
	int			m_iDamage;
	float		m_fStrMultiple;
	float		m_fContactTime;	
	DamageType  m_eDamageType;
};

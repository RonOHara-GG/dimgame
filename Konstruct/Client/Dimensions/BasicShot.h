#pragma once
#include "skill.h"
#include "Common/Utility/kpuVector.h"
#include "Projectile.h"

class Weapon;
class BasicShot :
	public Skill
{
public:
	BasicShot(void);
	virtual ~BasicShot(void);

	virtual bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:

	DamageType	m_eDamageType;
	float		m_fRangeMultiple;
	int			m_iDamageMultiple;
	
	

};

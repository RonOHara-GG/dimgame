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

	virtual bool Activate(PlayerCharacter* pSkillOwner);
	virtual bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	float		m_fRange;
	float		m_fDamage;
	DamageType	m_eDamageType;
	float		m_fRangeMultiple;
	float		m_fDamageMultiple;
	
	

};

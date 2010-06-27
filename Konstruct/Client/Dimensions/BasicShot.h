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
	//TODO: Remove these!!!!!!!!!!!!!!
	float		m_fRange;
	float		m_fMinRange;
	int			m_iDamage;
	DamageType	m_eDamageType;
	float		m_fRangeMultiple;
	int			m_iDamageMultiple;
	
	

};

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
	float		m_fRangeMultiple;
	float		m_fDamageMultiple;
	float		m_fRankMultipleMin;	
	float		m_fRankMultipleMax;	
	bool		m_bExecuted;
	DamageType	m_eDamageType;

};

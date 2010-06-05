#pragma once
#include "skill.h"
#include "Common/Utility/kpuVector.h"

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
	float		m_fDistTraveled;
	int			m_iRankMultipleMin;	
	int			m_iRankMultipleMax;	
	bool		m_bExecuted;

	kpuVector   m_vLocation;
	kpuVector   m_vDirection;
	Weapon*		m_pEquippedWeapon;
	Actor*		m_pLastHit;
};

#pragma once
#include "skill.h"

class PlagueCenserSkill :
	public Skill
{
public:
	PlagueCenserSkill(void);
	~PlagueCenserSkill(void);

	bool	Activate(PlayerCharacter* pSkillOwner);
	bool	Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	float		m_iRankMultipleMin;	
	float		m_iRankMultipleMax;	
	float		m_fDamage;
	DamageType  m_eDamageType;
	float		m_fRange;
	float		m_fRangeMod;		 
	int			m_iResistStr;
	float		m_fResistMod;
	float		m_fRadius;
	float		m_fRadiusMod;		 
};

#define MIN_RANGE 1
#define MIN_RESIST_STR 20
#define MIN_RADIUS 5
#define MIN_SPEED 20
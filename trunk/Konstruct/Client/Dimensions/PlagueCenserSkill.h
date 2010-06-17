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
	int			m_iRankMultipleMin;	
	int			m_iRankMultipleMax;	
	float		m_fDamage;
	DamageType  m_eDamageType;
	float		m_fRange;
	float		m_fRangeMod;		 
	float		m_fResistStr;
	float		m_fResistMod;
	float		m_fRadius;
	float		m_fRadiusMod;

	float		m_fMinRange;
	float		m_fMinRadius;
	float		m_fMinResist;
};
#define MIN_SPEED 20.0f
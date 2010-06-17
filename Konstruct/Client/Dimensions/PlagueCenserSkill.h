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
	int			m_iDamage;
	DamageType  m_eDamageType;
	float		m_fRange;
	float		m_fRangeMod;		 
	int			m_iResistStr;
	int			m_iResistMod;
	float		m_fRadius;
	float		m_fRadiusMod;

	float		m_fMinRange;
	float		m_fMinRadius;
	int			m_iMinResist;
};
#define MIN_SPEED 20.0f
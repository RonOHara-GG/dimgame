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
	int			GetRange() { return m_iMinRange + (m_iSkillRank / m_iRangeMod); }
	float		GetSpeed() { return m_fSpeed - m_iSkillRank; }

	
	DamageType  m_eDamageType;

	int			m_iMinRange;
	int			m_iRangeMod;	

	int			m_iMinResist;
	int			m_iResistMod;
	
	float		m_fRadiusMod;	
	float		m_fMinRadius;
	
};

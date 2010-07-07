#pragma once
#include "basicshot.h"

class AcidRain :
	public BasicShot
{
public:
	AcidRain(void);
	~AcidRain(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int			GetRange() { return m_iMinRange + m_iSkillRank /  m_iRangeMod; }
	float		GetSpeed() { return m_fSpeed + m_iSkillRank * m_fSpeedMod; }

	
	float		m_fRadiusMod;
	float		m_fMinRadius;	

	float		m_fSpeedMod;	
	
	int			m_iRangeMod;
	int			m_iMinRange;
	
	int			m_iMinResist;
	int			m_iResistMod;
};

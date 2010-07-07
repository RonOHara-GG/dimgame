#pragma once
#include "basicshot.h"

class ChangeAllegiance :
	public BasicShot
{
public:
	ChangeAllegiance(void);
	~ChangeAllegiance(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int			GetRange() { return m_iMinRange + m_iSkillRank /  m_iRangeMod; }
	
	int			m_iRangeMod;
	float		m_fRadiusMod;
	int			m_iResistMod;

	int			m_iMinRange;
	float		m_fMinRadius;
	int			m_iMinResist;

};

#pragma once
#include "basicshot.h"

class IceSpray :
	public BasicShot
{
public:
	IceSpray(void);
	~IceSpray(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int			GetRange() { return  m_iMinRange + m_iSkillRank /m_iRangeMod; }

	float		m_fArcMod;
	int			m_iRangeMod;
	int			m_iMinRange;
	int			m_iMinResist;
	int			m_iResistMod;
};

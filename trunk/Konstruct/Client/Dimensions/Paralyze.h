#pragma once
#include "basicshot.h"

class Paralyze :
	public BasicShot
{
public:
	Paralyze(void);
	~Paralyze(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int			GetRange() { return m_fMinRange + m_iSkillRank / m_iRangeMod; }

	float		m_fMinRadius;
	int			m_iRadiusMod;
	int			m_iRangeMod;
	int			m_iMinResist;
	int			m_iResistMod;

};

#pragma once
#include "basicshot.h"

class TakeLife :
	public BasicShot
{
public:
	TakeLife(void);
	~TakeLife(void);
	
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int GetRange() { return m_iMinRange + m_iSkillRank /  m_iRangeMod; }

	int		m_iRangeMod;
	int		m_iResistStr;
	float	m_fResistMod;	
	int		m_iMinRange;
	int		m_iMinResist;
	Actor*  m_pTarget;
};

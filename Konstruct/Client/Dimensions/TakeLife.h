#pragma once
#include "basicshot.h"

class TakeLife :
	public BasicShot
{
public:
	TakeLife(void);
	~TakeLife(void);
	
	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int		m_iRangeMod;
	int		m_iResistStr;
	float	m_fResistMod;	
	float	m_fMinRange;
	int		m_iMinResist;
	Actor*  m_pTarget;
};

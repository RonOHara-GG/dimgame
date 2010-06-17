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
	float	m_fResistStr;
	float	m_fResistMod;	
	float	m_fMinRange;
	float	m_fMinResist;
	Actor*  m_pTarget;
};

#pragma once
#include "basicshot.h"

class FireBall :
	public BasicShot
{
public:
	FireBall(void);
	~FireBall(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int			GetRange() { return m_iMinRange + m_iSkillRank; }
	
	float		m_fRadiusMod;
	int			m_iResistMod;

	int			m_iMinRange;
	float		m_fMinRadius;
	int			m_iMinResist;
};

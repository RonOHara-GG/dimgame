#pragma once
#include "spreadablebuff.h"

class HolyShield :
	public SpreadableBuff
{
public:
	HolyShield(PlayerCharacter* pSource);	
	~HolyShield(void);

	bool Activate(PlayerCharacter* pSkillOwner);

protected:
	SpreadableBuff* CopyBuff(Actor* pSpreadee);

	int		m_iDamageReduction;
	float	m_fRadiusMod;
	float	m_fMinRadius;
	
};

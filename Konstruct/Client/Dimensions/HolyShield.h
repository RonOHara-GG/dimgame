#pragma once
#include "spreadablebuff.h"

class HolyShield :
	public SpreadableBuff
{
public:
	HolyShield(PlayerCharacter* pSource);	
	~HolyShield(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	//bool Deactivate();
	//SpreadableBuff* CopyBuff();

protected:
	float	m_iDamageReduction;
	float	m_fRadiusMod;
	float	m_fMinRadius;
};

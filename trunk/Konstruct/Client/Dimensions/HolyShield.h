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
	float	m_fDamageReduction;
	float	m_fRadiusMod;
};

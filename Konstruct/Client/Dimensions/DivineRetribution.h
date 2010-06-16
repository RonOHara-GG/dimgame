#pragma once
#include "spreadablebuff.h"

class DivineRetribution :
	public SpreadableBuff
{
public:
	DivineRetribution(PlayerCharacter* pSource, PlayerCharacter* pSpreadTo);
	~DivineRetribution(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);
	bool Activate(PlayerCharacter* pSkillOwner);

protected:
	DamageType				m_eDamageType;
	float					m_fRadiusMod;
	PlayerCharacter*		m_pSpreadTo;
	
};

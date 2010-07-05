#pragma once
#include "spreadablebuff.h"

class DivineRetribution :
	public SpreadableBuff
{
public:
	DivineRetribution(PlayerCharacter* pSource, PlayerCharacter* pSpreadTo, float fRadius);
	DivineRetribution(void);
	~DivineRetribution(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);
	bool Activate(PlayerCharacter* pSkillOwner);

protected:
	DamageType				m_eDamageType;
	float					m_fRadiusMod;
	float					m_fMinRadius;
	PlayerCharacter*		m_pSpreadTo;
	
};

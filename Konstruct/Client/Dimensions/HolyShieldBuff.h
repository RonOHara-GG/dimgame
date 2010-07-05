#pragma once
#include "SpreadableBuff.h"

class HolyShieldBuff :
	public SpreadableBuff
{
public:
	HolyShieldBuff(Actor* pSource, Actor* pSpreadee, float fRadius, int iDmgReduction);
	~HolyShieldBuff(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int		m_iDamageReduction;
	float	m_fRadius;

	Actor* m_pSource;
};

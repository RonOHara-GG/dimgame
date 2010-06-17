#pragma once
#include "FireBall.h"

class LightningBolt :
	public FireBall
{
public:
	LightningBolt(void);
	~LightningBolt(void);

	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	float		m_fMinArcChance;
};

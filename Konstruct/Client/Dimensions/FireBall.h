#pragma once
#include "basicshot.h"

class FireBall :
	public BasicShot
{
public:
	FireBall(void);
	~FireBall(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	float		m_fRadius;
	float		m_fRadiusMod;
	float		m_iResistStr;
	float		m_iResistMod;

	float		m_fMinRange;
	float		m_fMinRadius;
	float		m_fMinResist;
};

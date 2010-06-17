#pragma once
#include "basicshot.h"

class IceSpray :
	public BasicShot
{
public:
	IceSpray(void);
	~IceSpray(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	float		m_fMaxCosSqrd;
	float		m_fArcMod;
	float		m_fRangeMod;
	float		m_fMinRange;
	float		m_iResistStr;
	float		m_fMinResist;
	float		m_iResistMod;
};
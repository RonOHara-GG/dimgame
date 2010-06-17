#pragma once
#include "basicshot.h"

class Paralyze :
	public BasicShot
{
public:
	Paralyze(void);
	~Paralyze(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	kpuVector	m_vTarget;
	bool		m_bTargetSelected;
	float		m_fRadius;
	float		m_fMinRadius;
	int			m_iRadiusMod;
	int			m_iRangeMod;
	float		m_iResistStr;
	float		m_fMinResist;
	float		m_iResistMod;

};

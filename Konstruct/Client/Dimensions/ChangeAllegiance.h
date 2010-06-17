#pragma once
#include "basicshot.h"

class ChangeAllegiance :
	public BasicShot
{
public:
	ChangeAllegiance(void);
	~ChangeAllegiance(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	kpuVector	m_vTarget;	
	bool		m_bTargetSelected;
	float		m_fRadius;
	int			m_iRangeMod;
	int			m_iRadiusMod;
	float		m_iResistStr;
	float		m_iResistMod;

	float		m_fMinRange;
	float		m_fMinRadius;
	float		m_fMinResist;

};

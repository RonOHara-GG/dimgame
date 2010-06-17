#pragma once
#include "basicshot.h"

class CreateRift :
	public BasicShot
{
public:
	CreateRift(void);
	~CreateRift(void);
	
	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	kpuVector	m_vTarget;	
	bool		m_bTargetSelected;
	float		m_fRadius;
	int			m_iRangeMod;
	int			m_iRadiusMod;
	float		m_fResistStr;
	float		m_fResistMod;
	float		m_fDuration;
};

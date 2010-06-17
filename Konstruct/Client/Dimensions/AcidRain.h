#pragma once
#include "basicshot.h"

class AcidRain :
	public BasicShot
{
public:
	AcidRain(void);
	~AcidRain(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	kpuVector	m_vTarget;
	bool		m_bTargetSelected;
	float		m_fRadius;
	int			m_iBaseDamage;
	int			m_iRadiusMod;
	int			m_iRangeMod;
	float		m_fSpeedMod;
	float		m_iResistStr;
	float		m_iResistMod;
	float		m_fElaspedSinceTick;

	float		m_fMinRange;
	float		m_fMinRadius;
	float		m_fMinResist;

};

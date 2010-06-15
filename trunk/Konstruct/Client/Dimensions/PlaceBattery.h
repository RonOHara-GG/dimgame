#pragma once
#include "skill.h"

class PlaceBattery :
	public Skill
{
public:
	PlaceBattery(void);
	~PlaceBattery(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);

protected:
	int		m_iTargetTile;
	float	m_fSpeedMod;
};

#pragma once
#include "skill.h"
#include "MissileBattery.h"

class PlaceBattery :
	public Skill
{
public:
	PlaceBattery(void);
	~PlaceBattery(void);

	bool Activate(PlayerCharacter* pSkillOwner);
	bool Update(PlayerCharacter* pSkillOwner, float fDeltaTime);
	

protected:
	float GetSpeed() { return m_fMaxSpeed - (m_fSpeedMod * m_iSkillRank); }
	int   GetRange() { return 1; }

	int									m_iTargetTile;
	float								m_fSpeedMod;
	float								m_fMaxSpeed;
	MissileBattery::BatteryType			m_eBatteryType;
};

#pragma once
#include "skill.h"
#include "weapon.h"


class WeaponSkill :
	public Skill
{
public:
	WeaponSkill(void);
	~WeaponSkill(void);
	
	bool Activate(PlayerCharacter* pSkillOwner);

	int GetDamageBonus() { return m_iDamageBonus; }
	float GetRangeBonus() { return m_fRangeBonus; }
	float GetRecoveryBonus() { return m_fRecoveryBonus; }
	float GetSpeedBonus() { return m_fSpeedBonus; }
	Weapon::eWeaponType GetUsage() { return m_eWeaponType; }

protected:
	int					m_iDamageBonus;
	float				m_fRangeBonus;
	float				m_fRecoveryBonus;
	float				m_fSpeedBonus;	

	//data that needs to be loaded
	Weapon::eWeaponType	m_eWeaponType;
	float				m_fRangeMod;
	float				m_fRecoveryMod;
	float				m_fSpeedMod;

};

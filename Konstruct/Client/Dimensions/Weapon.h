#pragma once

#include "equippable.h"
#include "DamageTypes.h"

class Actor;
class WeaponSkill;

enum eWeaponType
{
	eWT_Pistol,
	eWT_Rifle,
	eWT_AssultRifle,
	eWT_ShortBow,
	eWT_LongBow,
	eWT_CompoundBow,
	eWT_CrossBow,
	eWT_Ballista,
	eWT_RPG,
	eWT_SeekerMissle,
	eWT_CruiseMissle,
	eWT_BallisticMissle,
	eWT_Hammer,
	eWT_Staff
};

class Weapon: public Equippable
{
public:
	Weapon(void);
	~Weapon(void);

	void Update(float fGameTime);

	float GetRange();
	int	  GetDamage();
	float GetSpeed();
	float GetRecovery();

	_DamageType GetDamageType() { return m_eDamageType; }
	eWeaponType GetType() { return m_eWeaponType; }

	bool IsReady() { return m_bReady; }
	void SetReady(bool bReady) { m_bReady = bReady; }
	void Use(Actor* pTarget);
	void Equipped(WeaponSkill* pSkill) { m_pWeaponSkill = pSkill; }
	void Unequipped() { m_pWeaponSkill = 0; }

	

protected:

	float			m_fRange;
	int				m_iMinDamage;
	int				m_iMaxDamage;
	float			m_fRecovery;
	float			m_fElaspedRecov;
	float			m_fSpeed;
	DamageType		m_eDamageType;
	eWeaponType		m_eWeaponType;
	bool			m_bReady;			//True if weapon is recovered and ready to use
	
	WeaponSkill*	m_pWeaponSkill;
	
};

#pragma once

#include "equippable.h"
#include "DamageTypes.h"

class Actor;
class Weapon: public Equippable
{
public:
	Weapon(void);
	~Weapon(void);

	void Update(float fGameTime);

	float GetRange() { return m_fRange; }
	float GetDamage() { return m_fDamage; }
	float GetSpeed()	{ return m_fSpeed; }
	float GetRecovery() { return m_fRecovery; }

	_DamageType GetDamageType() { return m_eDamageType; }

	bool IsReady() { return m_bReady; }
	void SetReady(bool bReady) { m_bReady = bReady; }
	void Use(Actor* pTarget);

protected:

	float			m_fRange;
	float			m_fDamage;
	float			m_fRecovery;
	float			m_fElaspedRecov;
	float			m_fSpeed;
	DamageType		m_eDamageType;
	bool			m_bReady;			//True if weapon is recovered and ready to use
	
	
};

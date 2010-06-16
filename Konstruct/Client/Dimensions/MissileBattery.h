#pragma once
#include "PlayerPet.h"
#include "Weapon.h"

class MissileBattery :
	public PlayerPet, public Weapon
{
public:
	enum BatteryType
	{
		eBT_Seeker,
		eBT_Cruise,
		eBT_Ballistic,
	};

	MissileBattery(PlayerCharacter* pOwner, int iLevel, BatteryType eType);
	~MissileBattery(void);

	bool Update(float fGameTime);	
	void Unequip(PlayerCharacter* pPlayer);

protected:
	BatteryType		m_eBatteryType;
	bool			m_bDead;
};

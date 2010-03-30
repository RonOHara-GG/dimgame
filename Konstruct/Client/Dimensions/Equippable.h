#pragma once
#include "Item.h"

class PlayerCharacter;

class Equippable: public Item
{
public:
	Equippable(void);
	virtual ~Equippable(void);
	virtual void Equip(PlayerCharacter* pPlayer);
	virtual void UnEquip(PlayerCharacter* pPlayer);

protected:
	int			m_iStrBonus;
	int			m_iAgiBonus;
	int			m_iIntBonus;
	int			m_iConstBonus;
	int			m_iHealthBonus;
	int			m_iMentalBonus;

	//Resitances
	int			m_iCrushResBonus;
	int			m_iSlashResBonus;
	int			m_iPierceResBonus;
	int			m_iMentalResBonus;
	int			m_iHeatResBonus;
	int			m_iColdResBonus;
	int			m_iElectResBonus;
	int			m_iWaterResBonus;
	int			m_iAcidResBonus;
	int			m_iViralResBonus;
	int			m_iHolyResBonus;
	int			m_iDeathResBonus;
};

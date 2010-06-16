#include "StdAfx.h"
#include "Equippable.h"
#include "PlayerCharacter.h"

Equippable::Equippable(void)
{
	m_iStrBonus = 0;
	m_iAgiBonus = 0;
	m_iIntBonus = 0;
	m_iConstBonus = 0;
	m_iHealthBonus = 0;
	m_iMentalBonus = 0;

	//Resitances
	m_iCrushResBonus = 0;
	m_iSlashResBonus = 0;
	m_iPierceResBonus = 0;
	m_iMentalResBonus = 0;
	m_iHeatResBonus = 0;
	m_iColdResBonus = 0;
	m_iElectResBonus = 0;
	m_iWaterResBonus = 0;
	m_iAcidResBonus = 0;
	m_iViralResBonus = 0;
	m_iHolyResBonus = 0;
	m_iDeathResBonus = 0;
}

Equippable::~Equippable(void)
{
}

bool Equippable::MeetsRequirements(PlayerCharacter* pPlayer)
{
	if (pPlayer->GetStr() < m_iStrReq)
		return false;

	if (pPlayer->GetAgi() < m_iAgiReq)
		return false;

	if (pPlayer->GetConst() < m_iConstReq)
		return false;

	if (pPlayer->GetInt() < m_iIntReq)
		return false;

	return true;

}

void Equippable::Equip(PlayerCharacter *pPlayer)
{
	
	//Stat bonuses
	pPlayer->SetStr(pPlayer->GetStr() + m_iStrBonus);
	pPlayer->SetAgi(pPlayer->GetAgi() + m_iAgiBonus);
	pPlayer->SetInt(pPlayer->GetInt() + m_iIntBonus);
	pPlayer->SetConst(pPlayer->GetConst() + m_iStrBonus);
	pPlayer->SetMaxHealth(pPlayer->GetMaxHealth() + m_iHealthBonus);
	pPlayer->SetMaxMental(pPlayer->GetMaxMental() + m_iMentalBonus);

	//Resisit Bonues
	pPlayer->SetResist(pPlayer->GetResist(eDT_Crushing) + m_iCrushResBonus, eDT_Crushing);
	pPlayer->SetResist(pPlayer->GetResist(eDT_Slashing) + m_iSlashResBonus, eDT_Slashing);
	pPlayer->SetResist(pPlayer->GetResist(eDT_Piercing) + m_iPierceResBonus, eDT_Piercing);

	pPlayer->SetResist(pPlayer->GetResist(eDT_Mental) + m_iMentalResBonus, eDT_Mental);
	pPlayer->SetResist(pPlayer->GetResist(eDT_Heat) + m_iHeatResBonus, eDT_Heat);
	pPlayer->SetResist(pPlayer->GetResist(eDT_Cold) + m_iColdResBonus, eDT_Cold);

	pPlayer->SetResist(pPlayer->GetResist(eDT_Electrical) + m_iElectResBonus, eDT_Electrical);
	pPlayer->SetResist(pPlayer->GetResist(eDT_Water) + m_iWaterResBonus, eDT_Water);
	pPlayer->SetResist(pPlayer->GetResist(eDT_Acid) + m_iAcidResBonus, eDT_Acid);

	pPlayer->SetResist(pPlayer->GetResist(eDT_Viral) + m_iViralResBonus, eDT_Viral);
	pPlayer->SetResist(pPlayer->GetResist(eDT_Holy) + m_iHolyResBonus, eDT_Holy);
	pPlayer->SetResist(pPlayer->GetResist(eDT_Death) + m_iDeathResBonus, eDT_Death);

}

void Equippable::Unequip(PlayerCharacter *pPlayer)
{
	//Stat bonuses
	pPlayer->SetStr(pPlayer->GetStr() - m_iStrBonus);
	pPlayer->SetAgi(pPlayer->GetAgi() - m_iAgiBonus);
	pPlayer->SetInt(pPlayer->GetInt() - m_iIntBonus);
	pPlayer->SetConst(pPlayer->GetConst() - m_iStrBonus);
	pPlayer->SetMaxHealth(pPlayer->GetMaxHealth() - m_iHealthBonus);
	pPlayer->SetMaxMental(pPlayer->GetMaxMental() - m_iMentalBonus);

	//Resisit Bonues
	pPlayer->SetResist(pPlayer->GetResist(eDT_Crushing) - m_iCrushResBonus, eDT_Crushing);
	pPlayer->SetResist(pPlayer->GetResist(eDT_Slashing) - m_iSlashResBonus, eDT_Slashing);
	pPlayer->SetResist(pPlayer->GetResist(eDT_Piercing) - m_iPierceResBonus, eDT_Piercing);

	pPlayer->SetResist(pPlayer->GetResist(eDT_Mental) - m_iMentalResBonus, eDT_Mental);
	pPlayer->SetResist(pPlayer->GetResist(eDT_Heat) - m_iHeatResBonus, eDT_Heat);
	pPlayer->SetResist(pPlayer->GetResist(eDT_Cold) - m_iColdResBonus, eDT_Cold);

	pPlayer->SetResist(pPlayer->GetResist(eDT_Electrical) - m_iElectResBonus, eDT_Electrical);
	pPlayer->SetResist(pPlayer->GetResist(eDT_Water) - m_iWaterResBonus, eDT_Water);
	pPlayer->SetResist(pPlayer->GetResist(eDT_Acid) - m_iAcidResBonus, eDT_Acid);

	pPlayer->SetResist(pPlayer->GetResist(eDT_Viral) - m_iViralResBonus, eDT_Viral);
	pPlayer->SetResist(pPlayer->GetResist(eDT_Holy) - m_iHolyResBonus, eDT_Holy);
	pPlayer->SetResist(pPlayer->GetResist(eDT_Death) - m_iDeathResBonus, eDT_Death);


}
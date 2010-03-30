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
	pPlayer->SetCrushRes(pPlayer->GetCrushRes() + m_iCrushResBonus);
	pPlayer->SetSlashRes(pPlayer->GetSlashRes() + m_iSlashResBonus);
	pPlayer->SetPierceRes(pPlayer->GetPierceRes() + m_iPierceResBonus);

	pPlayer->SetMentalRes(pPlayer->GetMentalRes() + m_iMentalResBonus);
	pPlayer->SetHeatRes(pPlayer->GetHeatRes() + m_iHeatResBonus);
	pPlayer->SetColdRes(pPlayer->GetColdRes() + m_iColdResBonus);

	pPlayer->SetElectRes(pPlayer->GetElectRes() + m_iElectResBonus);
	pPlayer->SetWaterRes(pPlayer->GetWaterRes() + m_iWaterResBonus);
	pPlayer->SetAcidRes(pPlayer->GetAcidRes() + m_iAcidResBonus);

	pPlayer->SetViralRes(pPlayer->GetViralRes() + m_iViralResBonus);
	pPlayer->SetHolyRes(pPlayer->GetHolyRes() + m_iHolyResBonus);
	pPlayer->SetDeathRes(pPlayer->GetDeathRes() + m_iDeathResBonus);


}

void Equippable::UnEquip(PlayerCharacter *pPlayer)
{
	//Stat bonuses
	pPlayer->SetStr(pPlayer->GetStr() - m_iStrBonus);
	pPlayer->SetAgi(pPlayer->GetAgi() - m_iAgiBonus);
	pPlayer->SetInt(pPlayer->GetInt() - m_iIntBonus);
	pPlayer->SetConst(pPlayer->GetConst() - m_iStrBonus);
	pPlayer->SetMaxHealth(pPlayer->GetMaxHealth() - m_iHealthBonus);
	pPlayer->SetMaxMental(pPlayer->GetMaxMental() - m_iMentalBonus);

	//Resisit Bonues
	pPlayer->SetCrushRes(pPlayer->GetCrushRes() - m_iCrushResBonus);
	pPlayer->SetSlashRes(pPlayer->GetSlashRes() - m_iSlashResBonus);
	pPlayer->SetPierceRes(pPlayer->GetPierceRes() - m_iPierceResBonus);

	pPlayer->SetMentalRes(pPlayer->GetMentalRes() - m_iMentalResBonus);
	pPlayer->SetHeatRes(pPlayer->GetHeatRes() - m_iHeatResBonus);
	pPlayer->SetColdRes(pPlayer->GetColdRes() - m_iColdResBonus);

	pPlayer->SetElectRes(pPlayer->GetElectRes() - m_iElectResBonus);
	pPlayer->SetWaterRes(pPlayer->GetWaterRes() - m_iWaterResBonus);
	pPlayer->SetAcidRes(pPlayer->GetAcidRes() - m_iAcidResBonus);

	pPlayer->SetViralRes(pPlayer->GetViralRes() - m_iViralResBonus);
	pPlayer->SetHolyRes(pPlayer->GetHolyRes() - m_iHolyResBonus);
	pPlayer->SetDeathRes(pPlayer->GetDeathRes() - m_iDeathResBonus);


}
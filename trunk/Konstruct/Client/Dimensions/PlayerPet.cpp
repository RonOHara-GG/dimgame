#include "StdAfx.h"
#include "PlayerPet.h"
#include "PlayerPetAI.h"
#include "Enemy.h"

PlayerPet::PlayerPet(PlayerCharacter* pOwner, int iLevel)
{
	m_pOwner = pOwner;
	m_iLevel = iLevel;
	m_pAIBehavior = new PlayerPetAI(this);
}
PlayerPet::PlayerPet(Enemy* pConvert, PlayerCharacter* pOwner)
{
	PlayerPet(pOwner, pConvert->GetLevel());

	//copy all stats 
	m_iStr = pConvert->GetStr();				
	m_iAgi = pConvert->GetAgi();			
	m_iInt = pConvert->GetInt();		
	m_iConst = pConvert->GetConst();

	m_iMaxHealth = pConvert->GetMaxHealth();		
	m_iCurrentHealth = pConvert->GetCurrentHealth();	
	m_iMaxMental = pConvert->GetMaxMental();	
	m_iCurrentMental = pConvert->GetCurrentMental();
	
	m_iCrushRes =  pConvert->GetResist(eDT_Crushing);
	m_iSlashRes =  pConvert->GetResist(eDT_Slashing);
	m_iPierceRes =  pConvert->GetResist(eDT_Piercing);
	m_iMentalRes =  pConvert->GetResist(eDT_Mental);
	m_iHeatRes =  pConvert->GetResist(eDT_Heat);
	m_iColdRes =  pConvert->GetResist(eDT_Cold);
	m_iElectRes =  pConvert->GetResist(eDT_Electrical);
	m_iWaterRes =  pConvert->GetResist(eDT_Water);
	m_iAcidRes =  pConvert->GetResist(eDT_Acid);
	m_iViralRes =  pConvert->GetResist(eDT_Viral);
	m_iHolyRes =  pConvert->GetResist(eDT_Holy);
	m_iDeathRes =  pConvert->GetResist(eDT_Death);

	m_pModel = pConvert->GetModel();
	m_vHeading = pConvert->GetHeading();
	m_fBaseSpeed = pConvert->GetSpeed();
	SetLocation(pConvert->GetLocation());

}
PlayerPet::~PlayerPet(void)
{
}

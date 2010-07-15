#include "StdAfx.h"
#include "PlayerCharacter.h"
#include "Common/Graphics/kpgModel.h"
#include "Common/Graphics/kpgGeometryInstance.h"
#include "Common/Graphics/kpgUIManager.h"
#include "Skill.h"
#include "Weapon.h"
#include "Armor.h"
#include "PlayerClass.h"
#include "SkillCombo.h"
#include "Level.h"
#include "Grid.h"
#include "WeaponSkill.h"


PlayerCharacter::PlayerCharacter(void):Actor()
{
	m_pModel = new kpgModel();
	if(!m_pModel->Load("Assets\\Player\\brettsPlayer.dae") )//!m_pModel->Load("Assets\\Player\\PlayerCard.dae") )
	{
		delete m_pModel;
		m_pModel = 0;
	}

	kpgModel* pCollision = new kpgModel();

	if( pCollision->Load("Assets\\Player\\PlayerCollision.dae") )
		CalculateBoundingVolumes(pCollision);

	delete pCollision;

	ZeroMemory(m_aClasses, sizeof(m_aClasses));
	ZeroMemory(m_paInventory, sizeof(m_paInventory));

	m_fBaseSpeed = 10.0f;
	m_pActiveSkill = 0;

	m_pSkillCombos = new kpuArrayList<SkillCombo*>;

	//Create the player light source
	m_pLightSource = new kpgLight(kpgLight::eLT_Point);
	m_pLightSource->SetColor(kpuVector(0.75f, 0.75f, 0.75f, 1.0f));

	m_pEquippedWeapon = 0;
	m_fElaspedDefaultAtk = 0.0f;

	/*Init(m_pModel->GetBoundingBox().GetMin(), m_pModel->GetBoundingBox().GetMax());
	m_bBox.Move(GetLocation());
	m_bSphere.Move(GetLocation());*/

	SetFlag(ATTACKABLE | PLAYER);
	m_iCurrentHealth = m_iMaxHealth = 100;

	PlayerClass* pClass = new PlayerClass(eCL_Brawler, 100.0f);
	m_aClasses[0] = pClass;

	m_pWeaponSkills = new kpuLinkedList();
	m_plPlayerPets = new kpuLinkedList();	

	m_iStr = 100;
	m_iAgi = 100;

	m_iMoney = 1000000;

}

PlayerCharacter::PlayerCharacter(PlayerLoadStructure &playerData)
{
	//TODO: Load player from saved data



}

PlayerCharacter::PlayerCharacter(kpgModel* pModel, ePlayerClass eClass)
{
	m_pModel = pModel;

	kpgModel* pCollision = new kpgModel();

	if( pCollision->Load("Assets\\Player\\PlayerCollision.dae") )
		CalculateBoundingVolumes(pCollision);

	delete pCollision;

	ZeroMemory(m_aClasses, sizeof(m_aClasses));
	ZeroMemory(m_paInventory, sizeof(m_paInventory));

	m_fBaseSpeed = 10.0f;
	m_pActiveSkill = 0;

	m_pSkillCombos = new kpuArrayList<SkillCombo*>;

	//Create the player light source
	m_pLightSource = new kpgLight(kpgLight::eLT_Point);
	m_pLightSource->SetColor(kpuVector(0.75f, 0.75f, 0.75f, 1.0f));

	m_pEquippedWeapon = 0;
	m_fElaspedDefaultAtk = 0.0f;

	/*Init(m_pModel->GetBoundingBox().GetMin(), m_pModel->GetBoundingBox().GetMax());
	m_bBox.Move(GetLocation());
	m_bSphere.Move(GetLocation());*/

	SetFlag(ATTACKABLE);
	m_iCurrentHealth = m_iMaxHealth = 100;

	PlayerClass* pClass = new PlayerClass(eClass, 100.0f);
	m_aClasses[0] = pClass;

	m_pWeaponSkills = new kpuLinkedList();
	m_plPlayerPets = new kpuLinkedList();
	

	m_iStr = 100;
	m_iAgi = 100;
	m_iMoney = 1000000;

	m_pInventoryIcons = (char***)malloc(5 * sizeof(char*));

	for(int i = 0; i < 5; i++)
	{
		m_pInventoryIcons[i] = (char**)calloc(5, sizeof(char*));
	} 

	m_pInventoryList = (char***)malloc(INVENTORY_SIZE * sizeof(char*));

	for(int i = 0; i < INVENTORY_SIZE; i++)
	{
		m_pInventoryList[i] = (char**)calloc(3, sizeof(char*));
	} 
}

PlayerCharacter::~PlayerCharacter(void)
{
	for(int i = 0; i < NUM_PLAYER_CLASSES; i++)
	{
		if( m_aClasses[i] )
			delete m_aClasses[i];
	}

	if(m_pSkillCombos)
		delete m_pSkillCombos;

	if( m_pWeaponSkills )
		delete m_pWeaponSkills;

	delete m_pLightSource;
	delete m_pEquippedWeapon;

	for(int i = 0; i < INVENTORY_SIZE; i++)
	{
		if( m_paInventory[i] )
			delete m_paInventory[i];	
		
		if( m_pInventoryIcons[i % 5] )
		{
			free(m_pInventoryIcons[i % 5]);
			m_pInventoryIcons[i % 5] = 0;
		}

		if( m_pInventoryList[i] )
		{
			free(m_pInventoryList[i]);
			m_pInventoryList[i] = 0;
		}
	} 

	free(m_pInventoryIcons);
	free(m_pInventoryList);
}

bool PlayerCharacter::AddNewClass(ePlayerClass ePlayerClass, float fExpPercent)
{	
	if(m_aClasses[ePlayerClass])
		return false;

	//check exp split
	float fTotal = fExpPercent;

	for(int i = 0; i < NUMBER_OF_CLASSES; i++)
	{
		if(m_aClasses[i])
		{
			fTotal += m_aClasses[i]->GetExpSplit();
		}
	}

	if(fTotal != 1.0f)
		return false;
	
	m_aClasses[ePlayerClass] = new PlayerClass(ePlayerClass, fExpPercent);	

	return true;

}

void PlayerCharacter::AddPet(PlayerPet* pPet)
{
	m_plPlayerPets->AddTail(pPet);
}

void PlayerCharacter::RemovePet(PlayerPet* pPet)
{
	kpuLinkedList* pNext = m_plPlayerPets->Next();

	while( pNext )
	{
		PlayerPet* pNextPet = (PlayerPet*)pNext->GetPointer();

		if( pNextPet == pPet )
		{
			delete pNext;
			return;
		}

		pNext = pNext->Next();
	}

}
	
float PlayerCharacter::RemoveClass(ePlayerClass ePlayerClass)
{
	float fExpSplit = m_aClasses[ePlayerClass]->GetExpSplit();

	delete m_aClasses[ePlayerClass];

	return fExpSplit;
}

void PlayerCharacter::GainExp(int iExp)
{
	
	for(int i = 0; i < NUMBER_OF_CLASSES; i++)
	{
		if(m_aClasses[i])
		{
			if(m_aClasses[i]->GainExp(iExp))
				LevelUp();
		}
	}
}

void PlayerCharacter::LevelUp()
{
	m_iAttribPoints += ATTRIBUTE_POINTS_PER_LEVEL;
	m_iSkillPoints += SKILL_POINTS_FACTOR * GetLevel();
	ReconfigHealthMental();
}

bool PlayerCharacter::Update(float fDeltaTime)
{
	if( !m_pModel )
		return false;

	// Update player position
	UpdateMovement(fDeltaTime);

	kpuVector vPos = GetLocation();
	m_pLightSource->SetPosition(kpuVector(vPos.GetX(), -(vPos.GetY() + 0.9f), vPos.GetZ(), 6));	
	//kpgRenderer::GetInstance()->SetLight(0, m_pLightSource);

	//update any timers for weapons, skills, etc
	if( m_pEquippedWeapon )
		m_pEquippedWeapon->Update(fDeltaTime);
	else
	{
		//update default attack		
		if( m_fElaspedDefaultAtk < DEFAULT_ATTACK_RATE )
			m_fElaspedDefaultAtk += fDeltaTime;
	}

	UpdateSkills(fDeltaTime);

	return true;
}

void PlayerCharacter::UpdateMovement(float fDeltaTime)
{
	//check to make sure target doesn't contain a actor
	if( m_iDestinationTile > -1 && m_iCurrentPathNode > -1 )
	{
		Actor* pTileActor = g_pGameState->GetLevel()->GetGrid()->GetActor(m_aPathNodes[m_iCurrentPathNode]);

		//make sure the next tile is still walkable
		if( pTileActor && pTileActor != this )
		{		
			//rebuild path
			if( !BuildPathToDestination() )
			{
				//stay and move to center of tile
				SetNextMove(g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(GetLocation()));
			}
			
		}
	}

	Actor::UpdateMovement(fDeltaTime);
}

void PlayerCharacter::UpdateSkills(float fGameTime)
{
	//Update recovery timers of all skills
	for(int i = 0; i < NUMBER_OF_CLASSES; i++)
	{
		if(m_aClasses[i])
		{
			m_aClasses[i]->UpdateSkillTimers(fGameTime);
		}
	}

	//See if a combo is running
	for(int i = 0; i < m_pSkillCombos->Count(); i++)
	{
		SkillCombo* nextCombo = (*m_pSkillCombos)[i];

		if(nextCombo->IsRunning())
		{
			nextCombo->Use(fGameTime, this);
		}
	}

	//Update casting of current skill
	if(m_pActiveSkill)
	{
		if(!m_pActiveSkill->Update(this, fGameTime))		
			m_pActiveSkill = 0;
	}
}

bool PlayerCharacter::UseDefaultAttack(Actor* pTarget, Grid* pGrid)
{
	if(pTarget->HasFlag(ATTACKABLE))
	{
		if(m_pEquippedWeapon)
		{
			if(m_pEquippedWeapon->IsReady())
			{
				if(IsInRange(pTarget, m_pEquippedWeapon->GetRange()))
				{
					m_pEquippedWeapon->Use(pTarget);

					//Check target status
					CheckTargetStatus(pTarget);
					return true;
				}
			}
		}
		else
		{
			if( m_fElaspedDefaultAtk >= DEFAULT_ATTACK_RATE )
			{
				if(IsInRange(pTarget, DEFAULT_ATTACK_RANGE))
				{
					m_fElaspedDefaultAtk = 0.0f;
					pTarget->TakeDamage(m_iStr / 2 + DEFAULT_MELEE_DMG, eDT_Crushing);

					//Check target status
					CheckTargetStatus(pTarget);
					return true;				
				}
			}			

		}
	}

	return false;
}

float PlayerCharacter::GetRange()
{
	 return ( m_pEquippedWeapon ) ? m_pEquippedWeapon->GetRange()	: DEFAULT_MELEE_RANGE;
}

void PlayerCharacter::UseSkill(int iIndex, ePlayerClass ePlayerClass)
{
	if(m_aClasses[(int)ePlayerClass] && !m_pActiveSkill)
	{
		m_aClasses[(int)ePlayerClass]->GetSkill(iIndex)->Activate(this);		
	}
}

void PlayerCharacter::CheckTargetStatus(Actor* pTarget)
{
	if(!pTarget->IsAlive())
	{
		int iExp = (int)pow(pTarget->GetLevel(), EXP_EXPONENT);
		GainExp(iExp);
	}

}

void PlayerCharacter::AddSkillToComboAt(Skill *pSkill, int iCombo)
{
	(*m_pSkillCombos)[iCombo]->AddEnd(pSkill);
}

void PlayerCharacter::CreateCombo(Skill *pFirstSkill)
{
	//Make sure it was passed something
	if(pFirstSkill)
	{
		SkillCombo* newCombo = new SkillCombo();
		newCombo->AddEnd(pFirstSkill);

		m_pSkillCombos->Add(newCombo);
	}
}

void PlayerCharacter::SwapSkillsInCombo(int iCombo, int iOldIndex, int iNewIndex)
{
	(*m_pSkillCombos)[iCombo]->Swap(iOldIndex, iNewIndex);
}

void PlayerCharacter::RemoveSkillAt(int iCombo, int iIndex)
{
	(*m_pSkillCombos)[iCombo]->RemoveSkillAt(iIndex);
}

int PlayerCharacter::GetLevel()
{
	int iLevel = 0;

	for(int i = 0; i < NUMBER_OF_CLASSES; i++)
	{
		if(m_aClasses[i])
		{
			iLevel += m_aClasses[i]->GetLevel();
		}
	}

	return iLevel;
}

void PlayerCharacter::ReconfigHealthMental()
{
	int iLevel = GetLevel();

	m_iMaxHealth = m_iConst * iLevel * MENTAL_PER_LEVEL;
	m_iMaxMental = m_iInt * iLevel * MENTAL_PER_LEVEL;

}

void PlayerCharacter::SetInt(int iInt)
{
	m_iInt = iInt;
	ReconfigHealthMental();
}

void PlayerCharacter::SetConst(int iConst)
{
	m_iConst = iConst;
	ReconfigHealthMental();
}

bool PlayerCharacter::EquipWeapon(Weapon* weapon)
{
	//TODO: Implement equippng weapon from inventory

	if(!weapon->MeetsRequirements(this))
		return false;

	if( !UnequipWeapon() )
		return false;

	weapon->Equip(this);
	m_pEquippedWeapon = weapon;
	return true;
}

bool PlayerCharacter::UnequipWeapon()
{
	if(m_pEquippedWeapon)
	{
		//See if we can remove the currently equipped weapon
		for(int i = 0; i < INVENTORY_SIZE; i++)
		{
			if(!m_paInventory[i])
			{
				m_paInventory[i] = m_pEquippedWeapon;
				m_pEquippedWeapon->Unequip(this);
				m_pEquippedWeapon = 0;
				return true;
			}
		}

		if(m_pEquippedWeapon)
			return false;
	}

	return true;
}

bool PlayerCharacter::UnequipSecondary()
{
	if(m_pSecondaryWeapon)
	{
		//See if we can remove the currently equipped weapon
		for(int i = 0; i < INVENTORY_SIZE; i++)
		{
			if(!m_paInventory[i])
			{
				m_paInventory[i] = m_pSecondaryWeapon;
				m_pSecondaryWeapon->Unequip(this);
				m_pSecondaryWeapon = 0;
				return true;
			}
		}

		if(m_pSecondaryWeapon)
			return false;
	}

	return true;
}


bool PlayerCharacter::EquipSecondary(Weapon* weapon)
{
	if(!weapon->MeetsRequirements(this))
		return false;

	if( !UnequipSecondary() )
		return false;

	m_pSecondaryWeapon = weapon;
	weapon->Equip(this);
	return true;
}


WeaponSkill* PlayerCharacter::GetWeaponSkill(eWeaponType eType)
{
	kpuLinkedList* pNext = m_pWeaponSkills->Next();

	while( pNext )
	{
		WeaponSkill* pSkill = (WeaponSkill*)pNext->GetPointer();

		if( pSkill->GetUsage() == eType )
			return pSkill;

		pNext = pNext->Next();
	}

	return 0;
}

void PlayerCharacter::AddWeaponSkill(WeaponSkill* pSkill)
{	
	m_pWeaponSkills->AddTail(pSkill);
}

bool PlayerCharacter::AddItemToInventory(Item* pItem)
{
	//see if we have room for item, if we do add item and return true or return false
	for(int i = 0; i < INVENTORY_SIZE; i++)
	{
		if( !m_paInventory[i] )
		{
			m_paInventory[i] = pItem;

			//get row and column
			int iRow = i / 5;
			int iColumn = i % 5;
			//update item icons
			m_pInventoryIcons[iRow][iColumn] = pItem->GetIcon();
			return true;
		}
	}

	return false;
}

void PlayerCharacter::SetInventoryList()
{
	int j = 0;
	for(int i = 0; i < INVENTORY_SIZE; i++)
	{
		if( m_paInventory[i] )
		{
			m_pInventoryList[j][0] = m_paInventory[i]->GetIcon();				
			m_pInventoryList[j][1] = m_paInventory[i]->GetDescription();
			m_pInventoryList[j][2] = m_paInventory[i]->GetCostDisplay();	
			j++;
		}		
			
	}	

	m_pInventoryList[j][0] = 0;	

	//for now open basic item window
	kpgUIManager* pUIManager = g_pGameState->GetUIManager();		
	pUIManager->SetDataSource("ItemList", (char*)m_pInventoryList);

}

Item* PlayerCharacter::RemoveFromInventory(int iIndex)
{
	Item* pItem = m_paInventory[iIndex];
	m_paInventory[iIndex] = 0;
	//update inventory list
	SetInventoryList();
	return pItem;
}
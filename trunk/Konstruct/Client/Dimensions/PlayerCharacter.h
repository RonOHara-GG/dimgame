#pragma once

#include "Common/Utility/kpuArrayList.h"
#include "Common/Utility/kpuVector.h"
#include "Common/Graphics/kpgLight.h"
#include "Actor.h"
#include "Item.h"
#include "PlayerClass.h"
#include "LoadStructures.h"
#include "Weapon.h"


class Weapon;
class kpgRenderer;
class Skill;
class Grid;
class SkillCombo;
class Armor;
class WeaponSkill;
class PlayerPet;


#define INVENTORY_SIZE 25
#define NUM_PLAYER_CLASSES 8

#define ATTRIBUTE_POINTS_PER_LEVEL 5
#define EXP_EXPONENT 3.75
#define SKILL_POINTS_FACTOR 2
#define SPEED_AGILITY_RATIO 5
#define MENTAL_PER_LEVEL 5

#define DEFAULT_MELEE_DMG 5
#define DEFAULT_ATTACK_RATE 0.25f
#define DEFAULT_ATTACK_RANGE 1

class PlayerCharacter:public Actor
{
public:
	PlayerCharacter(void);
	PlayerCharacter(kpgModel* pModel, u32 uClassFlags);
	PlayerCharacter(PlayerLoadStructure& playerData);
	~PlayerCharacter(void);

	bool Update(float fDeltaTime);
	void UpdateMovement(float fDeltaTime);	

	void GainExp(int iExp); //Distributes exp over player's classes

	bool	AddNewClass(ePlayerClass ePlayerClass); //Adds a new class to the player, returns false if the class could not be added
	float	RemoveClass(ePlayerClass ePlayerClass); //Removes a class from the player and returns the amount of exp that class was getting
	bool	HasClass(ePlayerClass eClass) { return m_aClasses[eClass] != 0; }
	void	AdjustExpSplit(ePlayerClass eClass, float fExp);
	float	GetExpSplit(ePlayerClass eClass);
	void	SetExpSplit(ePlayerClass eClass, float fExp) { m_aClasses[eClass]->SetExpSplit(fExp); }
	int		ClassCount() { return m_iClassCount; }

	bool UseDefaultAttack(Actor* pTarget, Grid* pGrid);
	void UseSkill(int iIndex, ePlayerClass ePlayerClass);

	void SetActiveSkill(Skill* pSkill) { m_pActiveSkill = pSkill; }
	Skill* GetActiveSkill() { return m_pActiveSkill; }

	void SetTarget(Actor* pActor) { m_pTarget = pActor; }

	kpgLight* GetLight() { return m_pLightSource; }
	void SetLight(kpgLight* light) { m_pLightSource = light; }

	//If equipping or unequipping the item fails these functions return false
	bool EquipSecondary(Weapon* weapon);
	bool EquipWeapon(Weapon* weapon);
	bool UnequipWeapon();
	bool UnequipSecondary();

	bool EquipArmor(Armor* armor);
	bool UnequipArmor();

	bool AddItemToInventory(Item* pItem);

	WeaponSkill* GetWeaponSkill(eWeaponType eType);
	void AddWeaponSkill(WeaponSkill* pSkill);

	void AddPet(PlayerPet* pPet);
	void RemovePet(PlayerPet* pPet);

#pragma region Stat Accessors/Mutators

	int		GetLevel();
	float	GetSpeed() { return m_fBaseSpeed + 0.1f * (m_iAgi / SPEED_AGILITY_RATIO); }
	float 	GetRange();
	
	void	SetConst(int iConst);
	void	SetInt(int iInt);

	Weapon* GetEquippedWeapon()	{ return m_pEquippedWeapon; }
	Weapon* GetSecondaryWeapon() { return m_pSecondaryWeapon; }

	void	AddMoney(int iAmount) { m_iMoney += iAmount; m_iMoney < 0 ? m_iMoney = 0 : m_iMoney; }
	int		GetMoney() { return m_iMoney; }

	char*	GetInventoryIcons() { return (char*)m_pInventoryIcons; }
	void	SetInventoryList();
	Item*	RemoveFromInventory(int iIndex);

#pragma endregion

#pragma region ComboStuff

	void CreateCombo(Skill* pFirstSkill); //Creates a combo with a given skill
	void AddSkillToComboAt(Skill* pSkill, int iCombo);
	void SwapSkillsInCombo(int iCombo, int iOldIndex, int iNewIndex);
	void RemoveSkillAt(int iCombo, int iIndex);

#pragma endregion


protected:

	void ReconfigHealthMental();
	void CheckTargetStatus(Actor* pTarget);
	void LevelUp(); //Handles distirbution of skill and attribute points when a class reaches a new level
	void UpdateSkills(float fGameTime);

	//Leveling up stuff
	int							m_iAttribPoints;	
	int							m_iSkillPoints;

	//Inventory
	Armor*						m_pEqupipedArmor;
	Weapon*						m_pEquippedWeapon;
	Weapon*						m_pSecondaryWeapon;
	Item*						m_paInventory[INVENTORY_SIZE];
	char***						m_pInventoryIcons;
	char***						m_pInventoryList;

	int							m_iMoney;

	PlayerClass*				m_aClasses[NUM_PLAYER_CLASSES];
	int							m_iClassCount;

	Skill*						m_pActiveSkill; //The skill currently being cast/used
	kpuArrayList<SkillCombo*>*	m_pSkillCombos; //List of combos created
	kpuLinkedList*				m_pWeaponSkills;

	kpgLight*					m_pLightSource;

	//combat
	float						m_fElaspedDefaultAtk;

	kpuLinkedList*				m_plPlayerPets;
	
};



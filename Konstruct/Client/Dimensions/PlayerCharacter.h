#pragma once

#include "Common/Utility/kpuArrayList.h"
#include "Common/Utility/kpuVector.h"
#include "Common/Graphics/kpgLight.h"
#include "Actor.h"
#include "Item.h"
#include "PlayerClass.h"
//#include "SkillCombo.h"

class kpgRenderer;
class Skill;
class Weapon;
class Grid;
class SkillCombo;
class Weapon;
class Armor;

#define INVENTORY_SIZE 10

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
	~PlayerCharacter(void);

	bool Update(float fDeltaTime);
	void UpdateMovement(float fDeltaTime);	

	void GainExp(int iExp); //Distributes exp over player's classes

	bool AddNewClass(PlayerClass::Class eClass, float fExpPercent); //Adds a new class to the player, returns false if the class could not be added
	float RemoveClass(PlayerClass::Class eClass); //Removes a class from the player and returns the amount of exp that class was getting

	bool UseDefaultAttack(Actor* pTarget, Grid* pGrid);
	void UseSkill(int iIndex, PlayerClass::Class eClass, Actor* pTarget, Grid* pGrid);

	void SetActiveSkill(Skill* pSkill) { m_pActiveSkill = pSkill; }
	Skill* GetActiveSkill() { return m_pActiveSkill; }

	void SetTarget(Actor* pActor) { m_pTarget = pActor; }

	kpgLight* GetLight() { return m_pLightSource; }
	void SetLight(kpgLight* light) { m_pLightSource = light; }

	//If equipping or unequipping the item fails these functions return false
	bool EquipWeapon(Weapon* weapon);
	bool UnequipWeapon();

	bool EquipArmor(Armor* armor);
	bool UnequipArmor();

#pragma region Stat Accessors/Mutators

	int		GetLevel();
	float	GetSpeed() { return m_fBaseSpeed + 0.1 * (m_iAgi / SPEED_AGILITY_RATIO); }
	float 	GetRange();
	
	void	SetConst(int iConst);
	void	SetInt(int iInt);

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
	Item*						m_aInventory[INVENTORY_SIZE];

	PlayerClass*				m_aClasses[8];

	Skill*						m_pActiveSkill; //The skill currently being cast/used
	kpuArrayList<SkillCombo*>*	m_pSkillCombos; //List of combos created

	kpgLight*					m_pLightSource;

	//combat
	float						m_fElaspedDefaultAtk;
	
};



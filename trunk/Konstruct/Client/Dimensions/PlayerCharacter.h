#pragma once

#include "Common/Utility/kpuVector.h"
//#include "Common/Utility/kpuArrayList.h"
#include "Actor.h"
#include "Item.h"
#include "PlayerClass.h"
//#include "SkillCombo.h"

class kpgRenderer;
class Skill;
class Weapon;
class Grid;
class SkillCombo;
template<typename T> class kpuArrayList;


#define INVENTORY_SIZE 10

class PlayerCharacter:public Actor
{
public:
	PlayerCharacter(void);
	~PlayerCharacter(void);

	void Update(float fDeltaTime);
	

	int GetLevel();

	void GainExp(int iExp); //Distributes exp over player's classes

	bool AddNewClass(PlayerClass::Class eClass, float fExpPercent); //Adds a new class to the player, returns false if the class could not be added
	float RemoveClass(PlayerClass::Class eClass); //Removes a class from the player and returns the amount of exp that class was getting

	void UseDefaultAttack(Actor* pTarget, Grid* pGrid);
	void UseSkill(int iIndex, PlayerClass::Class eClass, Actor* pTarget, Grid* pGrid);

	void SetActiveSkill(Skill* pSkill) { m_pActiveSkill = pSkill; }
	Skill* GetActiveSkill() { return m_pActiveSkill; }

#pragma region ComboStuff

	void CreateCombo(Skill* pFirstSkill); //Creates a combo with a given skill
	void AddSkillToComboAt(Skill* pSkill, int iCombo);
	void SwapSkillsInCombo(int iCombo, int iOldIndex, int iNewIndex);
	void RemoveSkillAt(int iCombo, int iIndex);

#pragma endregion


protected:
	void LevelUp(); //Handles distirbution of skill and attribute points when a class reaches a new level
	void UpdateSkills(float fGameTime);

	//Leveling up stuff
	int							m_iAttribPoints;	

	//Inventory
	Weapon*						m_pEquippedWeapon;
	Item						m_aInventory[INVENTORY_SIZE];

	PlayerClass*				m_aClasses[8];

	Skill*						m_pActiveSkill; //The skill currently being cast/used
	kpuArrayList<SkillCombo*>*	m_pSkillCombos; //List of combos created
	
};

#define ATTRIBUTE_POINTS_PER_LEVEL 5

#define EXP_EXPONENT 3.75

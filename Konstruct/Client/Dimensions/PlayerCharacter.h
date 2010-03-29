#pragma once

#include "Common/Utility/kpuVector.h"
#include "Actor.h"
#include  "Item.h"
#include "PlayerClass.h"

class kpgRenderer;
class Skill;
class Weapon;
class Grid;

#define INVENTORY_SIZE 10

class PlayerCharacter:public Actor
{
public:
	PlayerCharacter(void);
	~PlayerCharacter(void);

	void Update(float fDeltaTime);
	void Draw(kpgRenderer* pRenderer);

	int GetLevel();

	void GainExp(int iExp); //Distributes exp over player's classes

	bool AddNewClass(PlayerClass::Class eClass, float fExpPercent); //Adds a new class to the player, returns false if the class could not be added
	float RemoveClass(PlayerClass::Class eClass); //Removes a class from the player and returns the amount of exp that class was getting

	void UseDefaultAttack(Actor* pTarget, Grid* pGrid);
	void UseSkill(int iIndex, PlayerClass::Class eClass, Actor* pTarget, Grid* pGrid);


protected:
	void LevelUp(); //Handles distirbution of skill and attribute points when a class reaches a new level

	//Leveling up stuff
	int				m_iAttribPoints;	

	//Inventory
	Weapon*			m_pEquippedWeapon;
	Item			m_aInventory[INVENTORY_SIZE];

	PlayerClass*	m_aClasses[8];
	
};

#define ATTRIBUTE_POINTS_PER_LEVEL 5

#define EXP_EXPONENT 3.75

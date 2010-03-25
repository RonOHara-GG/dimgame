#pragma once

#include "Common/Utility/kpuVector.h"
#include "Actor.h"
#include  "Item.h"

class kpgRenderer;
class Skill;

#define INVENTORY_SIZE 10

class PlayerCharacter:public Actor
{
public:
	PlayerCharacter(void);
	~PlayerCharacter(void);

	void Update(float fGameTime);
	void Draw(kpgRenderer* pRenderer);

	kpuVector GetLocation();
	void SetLocation(const kpuVector& vNewLoc);

	int GetLevel() { return m_iLevelBraw + m_iLevelArch + m_iLevelMark + m_iLevelMedi + m_iLevelOccu + m_iLevelPrie + m_iLevelRock + m_iLevelSword; }

	void GainExp(int iExp); //Distributes exp over player's classes

	enum PlayerClasses
	{
		eCL_Brawler,
		eCL_Swordsman,
		eCL_Archer,
		eCL_Marksman,
		eCL_Rocketeer,
		eCL_Medic,
		eCL_Priest,
		eCL_Occultist,
	};

protected:

	void UpdateMovement(float fGameTime); //Update the movement of the player

	void LevelUp(PlayerCharacter::PlayerClasses eClass); //Handles distirbution of skill and attribute points when a class reaches a new level

	//Leveling up stuff
	int				m_iAttribPoints;

	int				m_iLevelBraw;
	int				m_iPointsBraw;
	float			m_fCurrExpBraw;
	float			m_fNeedExpBraw;	
	float			m_fExpSplitBraw;

	int				m_iLevelSword;
	int				m_iPointsSword;
	float			m_fCurrExpSword;
	float			m_fNeedExpSword;	
	float			m_fExpSplitSword;

	int				m_iLevelArch;
	int				m_iPointsArch;
	float			m_fCurrExpArch;
	float			m_fNeedExpArch;	
	float			m_fExpSplitArch;

	int				m_iLevelMark;
	int				m_iPointsMark;
	float			m_fCurrExpMark;
	float			m_fNeedExpMark;
	float			m_fExpSplitMark;

	int				m_iLevelRock;
	int				m_iPointsRock;
	float			m_fCurrExpRock;
	float			m_fNeedExpRock;	
	float			m_fExpSplitRock;

	int				m_iLevelMedi;
	int				m_iPointsMedi;
	float			m_fCurrExpMedi;
	float			m_fNeedExpMedi;	
	float			m_fExpSplitMedi;

	int				m_iLevelPrie;
	int				m_iPointsPrie;
	float			m_fCurrExpPrie;
	float			m_fNeedExpPrie;	
	float			m_fExpSplitPrie;

	int				m_iLevelOccu;
	int				m_iPointsOccu;
	float			m_fCurrExpOccu;
	float			m_fNeedExpOccu;	
	float			m_fExpSplitOccu;


	//Inventory
	Item			m_aInventory[INVENTORY_SIZE];

	
};

#define ATTRIBUTE_POINTS_PER_LEVEL 5
#define SKILL_POINTS_MULTIPLE 2
#define EXP_EXPONENT 3.75

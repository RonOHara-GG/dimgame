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

	//Leveling up stuff
	int				m_iLevelBraw;
	float			m_fCurrExpBraw;
	float			m_fNeedExpBraw;	

	int				m_iLevelSword;
	float			m_fCurrExpSword;
	float			m_fNeedExpSword;	

	int				m_iLevelArch;
	float			m_fCurrExpArch;
	float			m_fNeedExpArch;	

	int				m_iLevelMark;
	float			m_fCurrExpMark;
	float			m_fNeedExpMark;	

	int				m_iLevelRock;
	float			m_fCurrExpRock;
	float			m_fNeedExpRock;	

	int				m_iLevelMedi;
	float			m_fCurrExpMedi;
	float			m_fNeedExpMedi;	

	int				m_iLevelPrie;
	float			m_fCurrExpPrie;
	float			m_fNeedExpPrie;	

	int				m_iLevelOccu;
	float			m_fCurrExpOccu;
	float			m_fNeedExpOccu;	


	//Inventory
	Item			m_aInventory[INVENTORY_SIZE];

	//Class Levels

	
};

#define ATTRIBUTE_POINTS_PER_LEVEL 5
#define SKILL_POINTS_MULTIPLE 2
#define EXP_EXPONENT 3.75

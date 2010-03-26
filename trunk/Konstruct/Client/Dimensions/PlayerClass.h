#pragma once

#include "Skill.h"

class PlayerClass
{
public:

	enum Class
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

	PlayerClass(Class eCL_Class, float fExpPercent);
	~PlayerClass(void);

	float GetExpSplit() { return m_fExpSplit; }
	float SetExpSplit(float fExpSplit) { m_fExpSplit = fExpSplit; }

	Skill GetSkill(int iIndex);		//Return a skill by given index	
	
	bool GainExp(int iExp);			//Returns true if level up from exp gain or false if not


private:
	int				m_iLevel;
	int				m_iSkillPoints;
	float			m_fCurrentExp;
	float			m_fNeededExp;	
	float			m_fExpSplit;

	int				m_iNumSkills;
	Skill*			m_paSkills;		//List of skills for the class

	//Class Initialization
	void ClassInit();
	void BrawlerInit();
	void ArcherInit();
	void SwordsInit();
	void MedicInit();
	void PriestInit();
	void MarksInit();
	void OccuInit();
	void RocketeerInit();

	void LevelUp();
};

#define MIN_EXP_SPLIT 0.05
#define NUMBER_OF_CLASSES 8
#define SKILL_POINTS_MULTIPLE 2
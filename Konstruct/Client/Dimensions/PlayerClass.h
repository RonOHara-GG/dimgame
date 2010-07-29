#pragma once

#include "Skill.h"

enum ePlayerClass
{
	eCL_Brawler,
	eCL_Swordsman,
	eCL_Archer,
	eCL_Marksman,
	eCL_Rocketeer,
	eCL_Medic,
	eCL_Priest,
	eCL_Occultist
};

#define NUM_OF_CLASSES 8

class PlayerClass
{
public:
	PlayerClass(ePlayerClass ePlayerClass, float fExpPercent);
	~PlayerClass(void);

	int				GetLevel() { return m_iLevel; }
	float			GetExpSplit() { return m_fExpSplit; }
	inline void		SetExpSplit(float fExpSplit);

	
	Skill*			GetSkill(int iIndex); //Get the skill of this class
	
	bool			GainExp(int iExp);			//Returns true if level up from exp gain or false if not

	void			UpdateSkillTimers(float fDeltaTime);

	static Skill*	GetSkill(ePlayerClass ePlayerClass, int iIndex);		//Return a skill by given index	and class
	static bool		LoadSkills();	//load all the skills for all classes

private:
	int						m_iLevel;
	
	float					m_fCurrentExp;
	float					m_fNeededExp;	
	float					m_fExpSplit;
	ePlayerClass					m_eClass;

	int						m_iNumSkills;
	Skill**					m_paSkills;
	static Skill**			m_paBrawlerSkills;		//List of skills for the class
	static Skill**			m_paSwordsmanSkills;	
	static Skill**			m_paArcherSkills;	
	static Skill**			m_paMarksmanSkills;		
	static Skill**			m_paRocketeerSkills;	
	static Skill**			m_paMedicSkills;
	static Skill**			m_paPriestSkills;		
	static Skill**			m_paOccultistSkills;	

	//ePlayerClass Initialization
	void ClassInit();

	void LevelUp() { m_iLevel++; }
};

#define MIN_EXP_SPLIT 0.05
#define NUMBER_OF_CLASSES 8

#define NUMBER_OF_BRAWLER_SKILLS 19
#define NUMBER_OF_ARCHER_SKILLS 12
#define NUMBER_OF_MEDIC_SKILLS 12
#define NUMBER_OF_MARKSMAN_SKILLS 13
#define NUMBER_OF_OCCULTIST_SKILLS 13
#define NUMBER_OF_PRIEST_SKILLS 12
#define NUMBER_OF_ROCKETEER_SKILLS 14
#define NUMBER_OF_SWORDSMAN_SKILLS 18

void PlayerClass::SetExpSplit(float fExpSplit)
{
	m_fExpSplit = fExpSplit;

	if( m_fExpSplit > 1.0f )
		m_fExpSplit = 1.0f;
	else if( m_fExpSplit < 0.05f )
		m_fExpSplit = 0.05f;
}

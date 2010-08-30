#include "StdAfx.h"
#include "PlayerClass.h"

#include "SpineCrank.h"

Skill**			PlayerClass::m_paBrawlerSkills = (Skill**)calloc(NUMBER_OF_BRAWLER_SKILLS, sizeof(Skill*));
Skill**			PlayerClass::m_paSwordsmanSkills = (Skill**)calloc(NUMBER_OF_SWORDSMAN_SKILLS, sizeof(Skill*));
Skill**			PlayerClass::m_paArcherSkills = (Skill**)calloc(NUMBER_OF_ARCHER_SKILLS, sizeof(Skill*));	
Skill**			PlayerClass::m_paMarksmanSkills = (Skill**)calloc(NUMBER_OF_MARKSMAN_SKILLS, sizeof(Skill*));		
Skill**			PlayerClass::m_paRocketeerSkills = (Skill**)calloc(NUMBER_OF_ROCKETEER_SKILLS, sizeof(Skill*));	
Skill**			PlayerClass::m_paMedicSkills = (Skill**)calloc(NUMBER_OF_MEDIC_SKILLS, sizeof(Skill*));
Skill**			PlayerClass::m_paPriestSkills = (Skill**)calloc(NUMBER_OF_PRIEST_SKILLS, sizeof(Skill*));		
Skill**			PlayerClass::m_paOccultistSkills = (Skill**)calloc(NUMBER_OF_OCCULTIST_SKILLS, sizeof(Skill*));

PlayerClass::PlayerClass(ePlayerClass eClass, float fExpPercent)
{

	switch(eClass)
	{
		case eCL_Brawler:
			{
				m_iNumSkills = NUMBER_OF_BRAWLER_SKILLS;
				m_paSkills = m_paBrawlerSkills;
				strcpy(m_szName, "Brawler");
				break;
			}
		case eCL_Archer:
			{
				m_iNumSkills = NUMBER_OF_ARCHER_SKILLS;
				m_paSkills = m_paArcherSkills;
				strcpy(m_szName, "Archer");
				break;
			}
		case eCL_Medic:
			{
				m_iNumSkills = NUMBER_OF_MEDIC_SKILLS;
				m_paSkills = m_paMedicSkills;
				strcpy(m_szName, "Medic");
				break;
			}
		case eCL_Marksman:
			{
				m_iNumSkills = NUMBER_OF_MARKSMAN_SKILLS;
				m_paSkills = m_paMarksmanSkills;
				strcpy(m_szName, "Marksman");
				break;
			}
		case eCL_Rocketeer:
			{
				m_iNumSkills = NUMBER_OF_ROCKETEER_SKILLS;
				m_paSkills = m_paRocketeerSkills;
				strcpy(m_szName, "Rocketeer");
				break;
			}
		case eCL_Occultist:
			{
				m_iNumSkills = NUMBER_OF_OCCULTIST_SKILLS;
				m_paSkills = m_paOccultistSkills;
				strcpy(m_szName, "Occultist");
				break;
			}
		case eCL_Priest:
			{
				m_iNumSkills = NUMBER_OF_PRIEST_SKILLS;
				m_paSkills = m_paPriestSkills;
				strcpy(m_szName,  "Priest");
				break;
			}
		case eCL_Swordsman:
			{
				m_iNumSkills = NUMBER_OF_SWORDSMAN_SKILLS;
				m_paSkills = m_paSwordsmanSkills;
				strcpy_s(m_szName,  "Swordsman");
				break;
			}
	}

	m_eClass = eClass;
	m_fExpSplit = fExpPercent;
	ClassInit();

	//Skill we are testing
	/*Skill* pSkill = new SpineCrank();
	m_paSkills[0] = pSkill;	
	m_paSkills[1] = 0;*/


}

PlayerClass::~PlayerClass(void)
{
	
}

void PlayerClass::UpdateSkillTimers(float fDeltaTime)
{
	for (int i = 0; i < m_iNumSkills; i++)
	{
		if(m_paSkills[i])
			m_paSkills[i]->UpdateTimers(fDeltaTime);
		else
			break;
	}
}

void PlayerClass::ClassInit()
{
	m_iLevel = 1;
	m_fCurrentExp = 0.0f;
	m_fNeededExp = 1000.0f;	
}

bool PlayerClass::GainExp(int iExp)
{
	m_fCurrentExp += iExp * m_fExpSplit;

	if(m_fCurrentExp >= m_fNeededExp)
	{
		m_fCurrentExp -= m_fNeededExp;
		LevelUp();

		return true;
	}

	return false;
}





Skill* PlayerClass::GetSkill(int iIndex)
{
	return m_paSkills[iIndex];
}

Skill* PlayerClass::GetSkill(ePlayerClass ePlayerClass, int iIndex)
{
	switch(ePlayerClass)
	{
		case eCL_Brawler:
			{				
				return m_paBrawlerSkills[iIndex];
			}
		case eCL_Archer:
			{
				return m_paArcherSkills[iIndex];				
			}
		case eCL_Medic:
			{
				return m_paMedicSkills[iIndex];
			}
		case eCL_Marksman:
			{
				return m_paMarksmanSkills[iIndex];
			}
		case eCL_Rocketeer:
			{
				return m_paRocketeerSkills[iIndex];
			}
		case eCL_Occultist:
			{
				return m_paOccultistSkills[iIndex];
			}
		case eCL_Priest:
			{
				return m_paPriestSkills[iIndex];
			}
		case eCL_Swordsman:
			{
				return m_paSwordsmanSkills[iIndex];
			}
	}

	
	return 0;

}

bool PlayerClass::LoadSkills()
{
	return false;
}


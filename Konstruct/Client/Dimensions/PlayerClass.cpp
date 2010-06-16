#include "StdAfx.h"
#include "PlayerClass.h"

#include "SpineCrank.h"

Skill**			PlayerClass::m_paBrawlerSkills = (Skill**)malloc(sizeof(Skill*) * NUMBER_OF_BRAWLER_SKILLS);
Skill**			PlayerClass::m_paSwordsmanSkills = (Skill**)malloc(sizeof(Skill*) * NUMBER_OF_SWORDSMAN_SKILLS);
Skill**			PlayerClass::m_paArcherSkills = (Skill**)malloc(sizeof(Skill*) * NUMBER_OF_ARCHER_SKILLS);	
Skill**			PlayerClass::m_paMarksmanSkills = (Skill**)malloc(sizeof(Skill*) * NUMBER_OF_MARKSMAN_SKILLS);		
Skill**			PlayerClass::m_paRocketeerSkills = (Skill**)malloc(sizeof(Skill*) * NUMBER_OF_ROCKETEER_SKILLS);	
Skill**			PlayerClass::m_paMedicSkills = (Skill**)malloc(sizeof(Skill*) * NUMBER_OF_MEDIC_SKILLS);
Skill**			PlayerClass::m_paPriestSkills = (Skill**)malloc(sizeof(Skill*) * NUMBER_OF_PRIEST_SKILLS);		
Skill**			PlayerClass::m_paOccultistSkills = (Skill**)malloc(sizeof(Skill*) * NUMBER_OF_OCCULTIST_SKILLS);

PlayerClass::PlayerClass(Class eClass, float fExpPercent)
{

	switch(eClass)
	{
		case eCL_Brawler:
			{
				m_iNumSkills = NUMBER_OF_BRAWLER_SKILLS;
				m_paSkills = m_paBrawlerSkills;
				break;
			}
		case eCL_Archer:
			{
				m_iNumSkills = NUMBER_OF_ARCHER_SKILLS;
				m_paSkills = m_paArcherSkills;
				break;
			}
		case eCL_Medic:
			{
				m_iNumSkills = NUMBER_OF_MEDIC_SKILLS;
				m_paSkills = m_paMedicSkills;
				break;
			}
		case eCL_Marksman:
			{
				m_iNumSkills = NUMBER_OF_MARKSMAN_SKILLS;
				m_paSkills = m_paMarksmanSkills;
				break;
			}
		case eCL_Rocketeer:
			{
				m_iNumSkills = NUMBER_OF_ROCKETEER_SKILLS;
				m_paSkills = m_paRocketeerSkills;
				break;
			}
		case eCL_Occultist:
			{
				m_iNumSkills = NUMBER_OF_OCCULTIST_SKILLS;
				m_paSkills = m_paOccultistSkills;
				break;
			}
		case eCL_Priest:
			{
				m_iNumSkills = NUMBER_OF_PRIEST_SKILLS;
				m_paSkills = m_paPriestSkills;
				break;
			}
		case eCL_Swordsman:
			{
				m_iNumSkills = NUMBER_OF_SWORDSMAN_SKILLS;
				m_paSkills = m_paSwordsmanSkills;
				break;
			}
	}

	m_eClass = eClass;
	m_fExpSplit = fExpPercent;
	ClassInit();

	//Skill we are testing
	Skill* pSkill = new SpineCrank();
	m_paSkills[0] = pSkill;	
	m_paSkills[1] = 0;


}

PlayerClass::~PlayerClass(void)
{
	if( m_paSkills )
		delete[] m_paSkills;
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
	m_iLevel = 0;
	m_fCurrentExp = 0.0f;
	m_fNeededExp = 0.0f;	
	m_fExpSplit = 100.0f;	
	m_paSkills = (Skill**)malloc(sizeof(Skill*) * m_iNumSkills);
	memset(m_paSkills, 0, sizeof(sizeof(Skill*) * m_iNumSkills));
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

void PlayerClass::LevelUp()
{
	m_iLevel++;
}

Skill* PlayerClass::GetSkill(int iIndex)
{
	return m_paSkills[iIndex];
}

Skill* PlayerClass::GetSkill(Class eClass, int iIndex)
{
	switch(eClass)
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


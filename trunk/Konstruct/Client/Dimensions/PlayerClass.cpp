#include "StdAfx.h"
#include "PlayerClass.h"

PlayerClass::PlayerClass(Class eClass, float fExpPercent)
{
	switch(eClass)
	{
		case eCL_Brawler:
			{
				BrawlerInit();
				break;
			}
		case eCL_Archer:
			{
				ArcherInit();
				break;
			}
		case eCL_Medic:
			{
				MedicInit();
				break;
			}
		case eCL_Marksman:
			{
				MarksInit();
				break;
			}
		case eCL_Rocketeer:
			{
				RocketeerInit();
				break;
			}
		case eCL_Occultist:
			{
				OccuInit();
				break;
			}
		case eCL_Priest:
			{
				PriestInit();
				break;
			}
		case eCL_Swordsman:
			{
				SwordsInit();
				break;
			}
	}

	m_fExpSplit = fExpPercent;
	ClassInit();
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
	}
}

void PlayerClass::ClassInit()
{
	m_iLevel = 0;
	m_iSkillPoints = 0;
	m_fCurrentExp = 0.0f;
	m_fNeededExp = 0.0f;	
	m_fExpSplit = 100.0f;
	m_paSkills = (Skill**)malloc(sizeof(Skill*) * m_iNumSkills);
}

void PlayerClass::BrawlerInit()
{
	m_iNumSkills = 19;	
}

void PlayerClass::ArcherInit()
{
	m_iNumSkills = 12;
	
}

void PlayerClass::MedicInit()
{
	m_iNumSkills = 12;
	
}

void PlayerClass::MarksInit()
{
	m_iNumSkills = 13;
	
}

void PlayerClass::OccuInit()
{
	m_iNumSkills = 13;
}

void PlayerClass::PriestInit()
{
	m_iNumSkills = 12;
}
	

void PlayerClass::RocketeerInit()
{
	m_iNumSkills = 14;

}

void PlayerClass::SwordsInit()
{
	m_iNumSkills = 18;
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
	m_iSkillPoints += SKILL_POINTS_MULTIPLE * m_iLevel;

}

Skill* PlayerClass::GetSkill(int iIndex)
{
	return m_paSkills[iIndex];
}
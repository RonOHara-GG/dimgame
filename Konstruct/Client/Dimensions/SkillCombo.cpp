#include "StdAfx.h"
#include "SkillCombo.h"
#include "PlayerCharacter.h"
#include "Skill.h"

SkillCombo::SkillCombo(void)
{
	m_pSkillList = new kpuArrayList<Skill*>;
	m_iNextSkill = 0;
	m_bRunning = false;
}

SkillCombo::~SkillCombo(void)
{
	delete m_pSkillList;
}

void SkillCombo::Swap(int iOldIndex, int iNewIndex)
{
	Skill* oldSkill = (*m_pSkillList)[iOldIndex];

	m_pSkillList->Override((*m_pSkillList)[iNewIndex], iOldIndex);
	m_pSkillList->Override(oldSkill, iNewIndex);
}

void SkillCombo::AddEnd(Skill *pSkill)
{
	m_pSkillList->Add(pSkill);
}

void SkillCombo::RemoveSkillAt(int iIndex)
{
	m_pSkillList->RemoveAt(iIndex);
}

void SkillCombo::Use(float fGameTime, PlayerCharacter* pPlayer)
{
	int iLastSkill = m_pSkillList->Count();

	//Make sure we don't try and use skills they aren't there
	if(m_iNextSkill >= iLastSkill)
	{
		m_bRunning = false;
		m_iNextSkill = 0;
	}

	//Run through skills and get next one to use
	for(int i = m_iNextSkill; i < iLastSkill; i++)
	{
		Skill* nextSkill = (*m_pSkillList)[i];

		if(pPlayer->GetActiveSkill() == 0 && nextSkill->IsReady())
		{
			m_bRunning = true;
			pPlayer->SetActiveSkill(nextSkill);
			m_iNextSkill++;
		}
	}
}

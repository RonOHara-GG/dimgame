#include "StdAfx.h"
#include "HeadButt.h"
#include "PlayerCharacter.h"
#include "Grid.h"
#include "Level.h"


HeadButt::HeadButt(void)
{
	m_pCurrentBearHug = 0;
}

HeadButt::~HeadButt(void)
{
}

bool HeadButt::Use(PlayerCharacter *pSkillOwner)
{
	if( !Strike::Use(pSkillOwner) )
		return false;

	//make sure we are in a bear hug
	Skill* pSkill = pSkillOwner->GetActiveSkill();

	if( strcmp(pSkill->GetName(), "Bear Hug") != 0 )
	{
		//use the bear hug skill somehow
		//pSkill = pSkillOwner->GetSkill("Bear Hug");
	}

	if( !pSkill->Use(pSkillOwner) )
		return false;

	m_pCurrentBearHug = pSkill;

	return true;
}

bool HeadButt::ApplyEffect(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	//Use bear hug
	m_pCurrentBearHug->ApplyEffect(pSkillOwner, fDeltaTime);

	//Try and headbutt
	if( Strike::ApplyEffect(pSkillOwner, fDeltaTime) )
	{
		//replace active skill with remaining hug
		pSkillOwner->SetActiveSkill(m_pCurrentBearHug);
		m_pCurrentBearHug = 0;		
	}

	return false;
}
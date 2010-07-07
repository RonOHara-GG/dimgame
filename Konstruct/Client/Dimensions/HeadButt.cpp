#include "StdAfx.h"
#include "HeadButt.h"
#include "BearHug.h"
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

bool HeadButt::Activate(PlayerCharacter *pSkillOwner)
{
	//make sure we are in a bear hug
	Skill* pSkill = pSkillOwner->GetActiveSkill();

	if( !Skill::Activate(pSkillOwner) )	
		return false;

	if( !pSkill || pSkill->GetIndex() != BearHug::m_siBearhugIndex )
	{
		//use the bear hug skill somehow
		pSkill = PlayerClass::GetSkill(eCL_Brawler, BearHug::m_siBearhugIndex);
	}

	if( !pSkill->Activate(pSkillOwner) )
		return false;

	m_pCurrentBearHug = pSkill;	

	return true;
}

bool HeadButt::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	m_fElaspedSinceCast += fDeltaTime;

	//Use bear hug
	if( !m_pCurrentBearHug->Update(pSkillOwner, fDeltaTime) )
	{
		m_fElaspedSinceCast = 0.0f;
		return false;
	}

	//Try and headbutt
	if( !Strike::Update(pSkillOwner, fDeltaTime) )
	{
		//replace active skill with remaining hug
		pSkillOwner->SetActiveSkill(m_pCurrentBearHug);
		m_pCurrentBearHug = 0;		
	}

	return true;
}
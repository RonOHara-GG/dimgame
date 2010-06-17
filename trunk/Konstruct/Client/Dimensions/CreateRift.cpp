#include "StdAfx.h"
#include "CreateRift.h"
#include "PlayerCharacter.h"
#include "Rift.h"

#define MIN_DURATION 2

CreateRift::CreateRift(void)
{
}

CreateRift::~CreateRift(void)
{
}


bool CreateRift::Activate(PlayerCharacter* pSkillOwner)
{
	if(m_bReady)
	{
		m_fElaspedSinceCast = 0.0f;
		
		m_fDuration = MIN_DURATION + m_iSkillRank;
		m_fRange = m_fMinRange + m_iSkillRank /  m_iRangeMod;
		m_fRadius = m_fMinRadius + (m_iSkillRank / m_iRadiusMod);
		m_fResistStr = m_fMinResist + (m_iSkillRank * m_fResistMod);		

		pSkillOwner->SetActiveSkill(this);
		
		m_bReady = false;
		m_bExecuted = false;	
		m_bTargetSelected = false;
		
		return true;		
	}

	return false;
}

bool CreateRift::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	//check input and see if mouse was clicked

	//if left mouse clicked then target selected and get the target	

	//if right mouse clicked then cancel skill
	//return false;


	if( m_bTargetSelected )
	{
		m_fElaspedSinceCast += fDeltaTime;		

		if( m_fElaspedSinceCast >= m_fSpeed )
		{
			//create rift
			pSkillOwner->AddPet(new Rift(pSkillOwner, 0, m_fRadius, m_vTarget, m_fResistStr, m_fDuration, m_eDamageType));
			m_bExecuted = true;
			return false;
		}
	}

	return true;
}
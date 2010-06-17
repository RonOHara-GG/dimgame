#include "StdAfx.h"
#include "Teleport.h"
#include "PlayerCharacter.h"

Teleport::Teleport(void)
{
}

Teleport::~Teleport(void)
{
}

bool Teleport::Activate(PlayerCharacter* pSkillOwner)
{
	if(m_bReady)
	{
		m_fElaspedSinceCast = 0.0f;
		
		m_fRange = m_fMinRange + m_iSkillRank;		

		pSkillOwner->SetActiveSkill(this);
		
		m_bReady = false;
		m_bExecuted = false;	
		m_bTargetSelected = false;
		m_pTarget = pSkillOwner;
		
		return true;		
	}

	return false;
}

bool Teleport::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
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
			kpuVector vVelocity = m_vTarget - m_pTarget->GetLocation();

			m_pTarget->Move(vVelocity);

			m_bExecuted = true;
			return false;		
			
		}
	}

	return true;
}

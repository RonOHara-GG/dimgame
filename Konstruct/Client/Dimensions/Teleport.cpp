#include "StdAfx.h"
#include "Teleport.h"
#include "PlayerCharacter.h"
#include "level.h"
#include "grid.h"

Teleport::Teleport(void)
{
}

Teleport::~Teleport(void)
{
}

bool Teleport::Activate(PlayerCharacter* pSkillOwner)
{
	if( m_bReady )
	{
		m_fElaspedSinceCast = 0.0f;
		
		//if actor is selected select him and wait for destination
		//if not move the skill owner to the desitanion tile

		if( VaildTarget(pSkillOwner) )
		{
			m_pTarget = pSkillOwner->GetTarget();
			m_iDestinationTile = -1;
		}
		else
		{
			m_pTarget = pSkillOwner;
			m_iDestinationTile = GetTargetTile();

			//see if target move is in range
			if( !InRange(pSkillOwner, m_iDestinationTile, GetRange()) )
				return false;

			//start animation

		}				

		pSkillOwner->SetActiveSkill(this);
		
		m_bReady = false;
		m_bExecuted = false;	
		
		return true;		
	}

	return false;
}

bool Teleport::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	if( m_iDestinationTile > -1 )
	{
		m_fElaspedSinceCast += fDeltaTime;		

		if( m_fElaspedSinceCast >= m_fSpeed )
		{
			//move target to desination
			kpuVector vLoc = kpuv_Zero;
			g_pGameState->GetLevel()->GetGrid()->GetTileLocation(m_iDestinationTile, vLoc);

			m_pTarget->SetLocation(vLoc);			

			m_bExecuted = true;
			return false;		
			
		}
	}
	//get location to move target if not skill owner
	

	return true;
}

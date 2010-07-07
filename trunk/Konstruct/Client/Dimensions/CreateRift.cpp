#include "StdAfx.h"
#include "CreateRift.h"
#include "PlayerCharacter.h"
#include "Rift.h"
#include "Level.h"
#include "Grid.h"

CreateRift::CreateRift(void)
{
}

CreateRift::~CreateRift(void)
{
}

bool CreateRift::Update(PlayerCharacter* pSkillOwner, float fDeltaTime)
{
	if( true )
	{
		float fDuration = m_fMinDuration + m_iSkillRank;
		float fRadius = m_fMinRadius + (m_iSkillRank / m_fRadiusMod);
		int iResistStr = m_iMinResist + (m_iSkillRank * m_iResistMod);	

		kpuVector vTarget = kpuv_Zero;
		g_pGameState->GetLevel()->GetGrid()->GetTileLocation(m_iTargetTile, vTarget);

		//create rift
		pSkillOwner->AddPet(new Rift(pSkillOwner, 0, fRadius, vTarget, iResistStr, fDuration, m_eDamageType));
		m_bExecuted = true;
		return false;
	}	

	return true;
}
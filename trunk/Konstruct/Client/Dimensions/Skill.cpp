#include "StdAfx.h"
#include "Skill.h"
#include "Actor.h"
#include "PlayerCharacter.h"
#include "Grid.h"
#include "Level.h"
#include "Common/input/kpiInputManager.h"


Skill::Skill(void)
{
	m_iSkillRank = 1;
	m_iRequiredLevel = 0;
	memset(m_szName, 0, sizeof(m_szName));
	m_fElaspedSinceCast = 0;
	m_fRecovery = 0;
	m_fElaspedRecovery = 0;
	m_bReady = true;
}

Skill::~Skill(void)
{
}

bool Skill::Activate(PlayerCharacter* pSkillOwner)
{
	if( m_bReady )
	{
		//Get ground target of mouse
		m_iTargetTile = GetTargetTile();

		//Check target actor if needed
		if( m_bTargetRequired )
		{
			if( !VaildTarget(pSkillOwner) )
				return false;

			m_pTarget = pSkillOwner->GetTarget();
		}

		//Check weapon if needed
		if( m_bWeaponRequired )
		{
			Weapon* pEquipped = 0;

			if( m_bSecondaryRequired )
				pEquipped = pSkillOwner->GetSecondaryWeapon();
			else
				pEquipped = pSkillOwner->GetEquippedWeapon();

			if( !pEquipped )
				return false;

			if( pEquipped->GetType() != m_eWeaponType )
				return false;

			m_pEquipped = pEquipped;
		}

		m_bReady = false;
		m_bExecuted = false;
		pSkillOwner->SetActiveSkill(this);
		return true;

	}

	return false;
}


void Skill::UpdateTimers(float fGameTime)
{
	if(!m_bReady)
	{
		m_fElaspedRecovery += fGameTime;

		if(m_fElaspedRecovery >= m_fRecovery)
		{
			m_bReady = true;
			m_fElaspedRecovery = 0.0f;
		}
	}
}

//bool Skill::FinishedCasting(float fGameTime)
//{
//	m_fElaspedSinceCast += fGameTime;
//
//	if(m_fElaspedSinceCast >= m_fSpeed)
//	{
//		m_fElaspedSinceCast = 0.0f;
//		return true;
//	}
//
//	return false;
//}

int Skill::GetTargetTile()
{
	kpPoint ptMouse = g_pInputManager->GetMouseLoc();
	kpuVector vTarget = kpuVector(ptMouse.m_iX, ptMouse.m_iY, 0.0f, 0.0f);
	g_pGameState->ScreenCordsToGameCords(vTarget);	

	Grid* pGrid = g_pGameState->GetLevel()->GetGrid();

	return pGrid->GetTileAtLocation(vTarget);
}

bool Skill::VaildTarget(PlayerCharacter* pSkillOwner)
{
	Actor* pTarget = pSkillOwner->GetTarget();
	m_pTarget = 0;

	//check target type
	if( !pTarget->HasFlag(m_uTargetType) )
	{
		if( m_uTargetType == PLAYER )
		{
			m_pTarget = pSkillOwner;
			return true;
		}

		return false;
	}

	//check range
	if( !InRange(pSkillOwner, pTarget, GetRange()) )
		return false;
	
	//check line of sight
	if( !pSkillOwner->InLineOfSight(pTarget, GetRange()) )
		return false;

	m_pTarget = pTarget;
	return true;
}

bool Skill::InRange(Actor* p1, Actor* p2, int iRange)
{
	return g_pGameState->GetLevel()->GetGrid()->DistanceSquared(p1, p2) > iRange * iRange;
}

bool Skill::InRange(int iTile1, int iTile2, int iRange)
{
	return g_pGameState->GetLevel()->GetGrid()->DistanceSquared(iTile1, iTile2) > iRange * iRange;
}

bool Skill::InRange(Actor* p1, int iTile1, int iRange)
{
	Grid* pGrid = g_pGameState->GetLevel()->GetGrid();
	int iTile2 = pGrid->GetTileAtLocation(p1->GetLocation());

	return pGrid->DistanceSquared(iTile1, iTile2) > iRange * iRange;
}
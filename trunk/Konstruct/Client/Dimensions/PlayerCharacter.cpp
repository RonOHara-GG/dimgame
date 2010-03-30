#include "StdAfx.h"
#include "PlayerCharacter.h"
#include "Common/Graphics/kpgModel.h"
#include "Common/Graphics/kpgGeometryInstance.h"
#include "Skill.h"
#include "Weapon.h"
#include "PlayerClass.h"
#include "SkillCombo.h"


PlayerCharacter::PlayerCharacter(void):Actor()
{
	m_pModel = new kpgModel();
	if( !m_pModel->Load("Assets\\Player\\PlayerCard.dae") )
	{
		delete m_pModel;
		m_pModel = 0;
	}

	ZeroMemory(m_aClasses, sizeof(m_aClasses));

	m_fSpeed = 1.3f;

	m_pSkillCombo = 0;

}

PlayerCharacter::~PlayerCharacter(void)
{
	if(m_pModel)
		delete m_pModel;

	if(m_aClasses)
		delete[] m_aClasses;

	if(m_pSkillCombo)
		delete m_pSkillCombo;
}

bool PlayerCharacter::AddNewClass(PlayerClass::Class eClass, float fExpPercent)
{	
	if(m_aClasses[eClass])
		return false;

	//check exp split
	float fTotal = fExpPercent;

	for(int i = 0; i < NUMBER_OF_CLASSES; i++)
	{
		if(m_aClasses[i])
		{
			fTotal += m_aClasses[i]->GetExpSplit();
		}
	}

	if(fTotal != 1.0f)
		return false;
	
	m_aClasses[eClass] = new PlayerClass(eClass, fExpPercent);	

	return true;

}

float PlayerCharacter::RemoveClass(PlayerClass::Class eClass)
{
	float fExpSplit = m_aClasses[eClass]->GetExpSplit();

	delete m_aClasses[eClass];

	return fExpSplit;
}

void PlayerCharacter::GainExp(int iExp)
{
	
	for(int i = 0; i < NUMBER_OF_CLASSES; i++)
	{
		if(m_aClasses[i])
		{
			if(m_aClasses[i]->GainExp(iExp))
				LevelUp();
		}
	}
}

void PlayerCharacter::LevelUp()
{
	m_iAttribPoints += ATTRIBUTE_POINTS_PER_LEVEL;
}

void PlayerCharacter::Update(float fDeltaTime)
{
	if( !m_pModel )
		return;

	// Update player position
	UpdateMovement(fDeltaTime);
}

void PlayerCharacter::UpdateSkills(float fGameTime)
{
	for(int i = 0; i < NUMBER_OF_CLASSES; i++)
	{
		if(m_aClasses[i])
		{
			m_aClasses[i]->UpdateSkillTimers(fGameTime);
		}
	}

	//See if a combo is running
	if(m_pSkillCombo->IsRunning())
	{
		m_pSkillCombo->Use(fGameTime, this);
	}

	//Update casting of current skill
	if(m_pActiveSkill)
	{
		if(m_pActiveSkill->FinishedCasting(fGameTime))
		{
			m_pActiveSkill->ApplyEffect(m_pTarget);
			m_pTarget = 0;
		}
	}


}

void PlayerCharacter::Draw(kpgRenderer* pRenderer)
{
	if( m_pModel )
		m_pModel->Draw(pRenderer);
}

void PlayerCharacter::UseDefaultAttack(Actor* pTarget, Grid* pGrid)
{
	if(pTarget->Attackable())
	{
		if(m_pEquippedWeapon->IsReady())
		{
			if(IsInRange(pTarget, m_pEquippedWeapon->GetRange() ,pGrid))
			{
				m_pEquippedWeapon->Use(pTarget);
			}
		}
	}
}

void PlayerCharacter::UseSkill(int iIndex, PlayerClass::Class eClass, Actor* pTarget, Grid* pGrid)
{
	if(m_aClasses[(int)eClass])
		m_aClasses[(int)eClass]->GetSkill(iIndex)->Use(pTarget, this, pGrid);
}

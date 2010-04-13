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
	if(!m_pModel->Load("Assets\\Player\\brettsPlayer.dae") )//!m_pModel->Load("Assets\\Player\\PlayerCard.dae") )
	{
		delete m_pModel;
		m_pModel = 0;
	}

	ZeroMemory(m_aClasses, sizeof(m_aClasses));

	m_fSpeed = 1.3f;

	m_pSkillCombos = new kpuArrayList<SkillCombo*>;

	//Create the player light source
	m_pLightSource = new kpgLight(kpgLight::eLT_Point);
	m_pLightSource->SetColor(kpuVector(0.75f, 0.75f, 0.75f, 1.0f));

	m_pEquippedWeapon = 0;

}

PlayerCharacter::~PlayerCharacter(void)
{
	if(m_pModel)
		delete m_pModel;

	if(m_aClasses)
		delete[] m_aClasses;

	if(m_pSkillCombos)
		delete m_pSkillCombos;

	delete m_pLightSource;
	delete m_pEquippedWeapon;

	delete[] m_aInventory;
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

bool PlayerCharacter::Update(float fDeltaTime)
{
	if( !m_pModel )
		return false;

	// Update player position
	UpdateMovement(fDeltaTime);

	kpuVector vPos = GetLocation();
	m_pLightSource->SetPosition(kpuVector(vPos.GetX(), -(vPos.GetY() + 0.9f), vPos.GetZ(), 6));	
	//kpgRenderer::GetInstance()->SetLight(0, m_pLightSource);

	return true;
}

void PlayerCharacter::UpdateSkills(float fGameTime)
{
	//Update recovery timers of all skills
	for(int i = 0; i < NUMBER_OF_CLASSES; i++)
	{
		if(m_aClasses[i])
		{
			m_aClasses[i]->UpdateSkillTimers(fGameTime);
		}
	}

	//See if a combo is running
	for(int i = 0; i < m_pSkillCombos->Count(); i++)
	{
		SkillCombo* nextCombo = (*m_pSkillCombos)[i];

		if(nextCombo->IsRunning())
		{
			nextCombo->Use(fGameTime, this);
		}
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

bool PlayerCharacter::UseDefaultAttack(Actor* pTarget, Grid* pGrid)
{
	if(pTarget->Attackable())
	{
		if(m_pEquippedWeapon)
		{
			if(m_pEquippedWeapon->IsReady())
			{
				if(IsInRange(pTarget, m_pEquippedWeapon->GetRange() ,pGrid))
				{
					m_pEquippedWeapon->Use(pTarget);
					return true;
				}
			}
		}
	}

	return false;
}

void PlayerCharacter::UseSkill(int iIndex, PlayerClass::Class eClass, Actor* pTarget, Grid* pGrid)
{
	if(m_aClasses[(int)eClass])
		m_aClasses[(int)eClass]->GetSkill(iIndex)->Use(pTarget, this, pGrid);
}

void PlayerCharacter::AddSkillToComboAt(Skill *pSkill, int iCombo)
{
	(*m_pSkillCombos)[iCombo]->AddEnd(pSkill);
}

void PlayerCharacter::CreateCombo(Skill *pFirstSkill)
{
	//Make sure it was passed something
	if(pFirstSkill)
	{
		SkillCombo* newCombo = new SkillCombo();
		newCombo->AddEnd(pFirstSkill);

		m_pSkillCombos->Add(newCombo);
	}
}

void PlayerCharacter::SwapSkillsInCombo(int iCombo, int iOldIndex, int iNewIndex)
{
	(*m_pSkillCombos)[iCombo]->Swap(iOldIndex, iNewIndex);
}

void PlayerCharacter::RemoveSkillAt(int iCombo, int iIndex)
{
	(*m_pSkillCombos)[iCombo]->RemoveSkillAt(iIndex);
}

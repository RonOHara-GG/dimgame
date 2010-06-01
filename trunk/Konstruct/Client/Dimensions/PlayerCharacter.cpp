#include "StdAfx.h"
#include "PlayerCharacter.h"
#include "Common/Graphics/kpgModel.h"
#include "Common/Graphics/kpgGeometryInstance.h"
#include "Skill.h"
#include "Weapon.h"
#include "Armor.h"
#include "PlayerClass.h"
#include "SkillCombo.h"
#include "Level.h"
#include "Grid.h"


PlayerCharacter::PlayerCharacter(void):Actor()
{
	m_pModel = new kpgModel();
	if(!m_pModel->Load("Assets\\Player\\brettsPlayer.dae") )//!m_pModel->Load("Assets\\Player\\PlayerCard.dae") )
	{
		delete m_pModel;
		m_pModel = 0;
	}

	kpgModel* pCollision = new kpgModel();

	if( pCollision->Load("Assets\\Player\\PlayerCollision.dae") )
		CalculateBoundingVolumes(pCollision);

	delete pCollision;

	ZeroMemory(m_aClasses, sizeof(m_aClasses));

	m_fBaseSpeed = 10.0f;

	m_pSkillCombos = new kpuArrayList<SkillCombo*>;

	//Create the player light source
	m_pLightSource = new kpgLight(kpgLight::eLT_Point);
	m_pLightSource->SetColor(kpuVector(0.75f, 0.75f, 0.75f, 1.0f));

	m_pEquippedWeapon = 0;
	m_fElaspedDefaultAtk = 0.0f;

	/*Init(m_pModel->GetBoundingBox().GetMin(), m_pModel->GetBoundingBox().GetMax());
	m_bBox.Move(GetLocation());
	m_bSphere.Move(GetLocation());*/

	m_bAttackable = true;
	m_fCurrentHealth = m_fMaxHealth = 100;

}

PlayerCharacter::~PlayerCharacter(void)
{
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
	m_iSkillPoints += SKILL_POINTS_FACTOR * GetLevel();
	ReconfigHealthMental();
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

	//update any timers for weapons, skills, etc
	if( m_pEquippedWeapon )
		m_pEquippedWeapon->Update(fDeltaTime);
	else
	{
		//update default attack		
		if( m_fElaspedDefaultAtk < DEFAULT_ATTACK_RATE )
			m_fElaspedDefaultAtk += fDeltaTime;
	}

	return true;
}

void PlayerCharacter::UpdateMovement(float fDeltaTime)
{
	//check to make sure target doesn't contain a actor
	if( m_iDestinationTile > -1 && m_iCurrentPathNode > -1 )
	{
		Actor* pTileActor = g_pGameState->GetLevel()->GetGrid()->GetActor(m_aPathNodes[m_iCurrentPathNode]);

		//make sure the next tile is still walkable
		if( pTileActor && pTileActor != this )
		{		
			//rebuild path
			if( !BuildPathToDestination() )
			{
				//stay and move to center of tile
				SetNextMove(g_pGameState->GetLevel()->GetGrid()->GetTileAtLocation(GetLocation()));
			}
			
		}
	}

	Actor::UpdateMovement(fDeltaTime);
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
		if(m_pActiveSkill->ApplyEffect(this, fGameTime))		
			m_pActiveSkill = 0;
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
				if(IsInRange(pTarget, m_pEquippedWeapon->GetRange()))
				{
					m_pEquippedWeapon->Use(pTarget);

					//Check target status
					CheckTargetStatus(pTarget);
					return true;
				}
			}
		}
		else
		{
			if( m_fElaspedDefaultAtk >= DEFAULT_ATTACK_RATE )
			{
				if(IsInRange(pTarget, DEFAULT_ATTACK_RANGE))
				{
					m_fElaspedDefaultAtk = 0.0f;
					pTarget->TakeDamage(m_iStr * 0.5f + DEFAULT_MELEE_DMG, eDT_Crushing);

					//Check target status
					CheckTargetStatus(pTarget);
					return true;				
				}
			}			

		}
	}

	return false;
}

float PlayerCharacter::GetRange()
{
	 return ( m_pEquippedWeapon ) ? m_pEquippedWeapon->GetRange()	: DEFAULT_MELEE_RANGE;
}

void PlayerCharacter::UseSkill(int iIndex, PlayerClass::Class eClass, Actor* pTarget, Grid* pGrid)
{
	if(m_aClasses[(int)eClass])
	{
		m_aClasses[(int)eClass]->GetSkill(iIndex)->Use(this);
		CheckTargetStatus(pTarget);
	}
}

void PlayerCharacter::CheckTargetStatus(Actor* pTarget)
{
	if(!pTarget->IsAlive())
	{
		int iExp = pow(pTarget->GetLevel(), EXP_EXPONENT);
		GainExp(iExp);
	}

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

int PlayerCharacter::GetLevel()
{
	int iLevel = 0;

	for(int i = 0; i < NUMBER_OF_CLASSES; i++)
	{
		if(m_aClasses[i])
		{
			iLevel += m_aClasses[i]->GetLevel();
		}
	}

	return iLevel;
}

void PlayerCharacter::ReconfigHealthMental()
{
	int iLevel = GetLevel();

	m_fMaxHealth = m_iConst * iLevel * MENTAL_PER_LEVEL;
	m_fMaxMental = m_iInt * iLevel * MENTAL_PER_LEVEL;

}

void PlayerCharacter::SetInt(int iInt)
{
	m_iInt = iInt;
	ReconfigHealthMental();
}

void PlayerCharacter::SetConst(int iConst)
{
	m_iConst = iConst;
	ReconfigHealthMental();
}

bool PlayerCharacter::EquipWeapon(Weapon* weapon)
{
	//TODO: Implement equippng weapon from inventory

	if(!weapon->MeetsRequirements(this))
		return false;

	if(m_pEquippedWeapon)
	{
		//See if we can remove the currently equipped weapon
		for(int i = 0; i < INVENTORY_SIZE; i++)
		{
			if(!m_aInventory[i])
			{
				m_aInventory[i] = m_pEquippedWeapon;
				m_pEquippedWeapon = 0;
			}
		}

		if(m_pEquippedWeapon)
			return false;
	}

	
	

	return true;
}

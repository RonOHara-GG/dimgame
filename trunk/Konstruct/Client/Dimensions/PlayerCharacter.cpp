#include "StdAfx.h"
#include "PlayerCharacter.h"
#include "Common/Graphics/kpgModel.h"
#include "Common/Graphics/kpgGeometryInstance.h"
#include "Skill.h"

PlayerCharacter::PlayerCharacter(void):Actor()
{
	m_pModel = new kpgModel();
	if( !m_pModel->Load("Assets\\Player\\PlayerCard.dae") )
	{
		delete m_pModel;
		m_pModel = 0;
	}	

}

void PlayerCharacter::PlayerInit()
{
	ZeroMemory(m_aClasses, sizeof(m_aClasses));

	m_fSpeed = 1.3f;

}

PlayerCharacter::~PlayerCharacter(void)
{
	if(m_pModel)
		delete m_pModel;

	if(m_aClasses)
		delete[] m_aClasses;
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
		else
			break;
	}

	if(fTotal != 1.0f)
		return false;
	
	m_aClasses[eClass] = new PlayerClass(eClass, fExpPercent);	

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
		else
			break;
	}



}

void PlayerCharacter::LevelUp()
{
	m_iAttribPoints += ATTRIBUTE_POINTS_PER_LEVEL;
}

void PlayerCharacter::Update(float fGameTime)
{
	if( !m_pModel )
		return;

	// Update player position
	UpdateMovement(fGameTime);
}


void PlayerCharacter::UpdateMovement(float fGameTime)
{
	kpuVector vPlayerToTarget = m_vMoveTarget - GetLocation();

	if( vPlayerToTarget.Dot(vPlayerToTarget) > 1 )
	{
		m_vDirection = kpuVector::Normalize(vPlayerToTarget);		
	}
	else
		m_vMoveTarget = GetLocation();

	m_vVelocity = m_vDirection * m_fSpeed;

	SetLocation(GetLocation() + (m_vVelocity * fGameTime));
	
	m_vDirection = kpuv_Zero;
}



void PlayerCharacter::Draw(kpgRenderer* pRenderer)
{
	if( m_pModel )
		m_pModel->Draw(pRenderer);
}

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

	m_iLevelBraw = 0;
	m_fCurrExpBraw = 0.0f;
	m_fNeedExpBraw = 0.0f;	

	m_iLevelSword = 0;
	m_fCurrExpSword = 0.0f;
	m_fNeedExpSword = 0.0f;	

	m_iLevelArch = 0;
	m_fCurrExpArch = 0.0f;
	m_fNeedExpArch = 0.0f;	

	m_iLevelMark = 0;
	m_fCurrExpMark = 0.0f;
	m_fNeedExpMark = 0.0f;	

	m_iLevelRock = 0;
	m_fCurrExpRock = 0.0f;
	m_fNeedExpRock = 0.0f;	

	m_iLevelMedi = 0;
	m_fCurrExpMedi = 0.0f;
	m_fNeedExpMedi = 0.0f;	

	m_iLevelPrie = 0;
	m_fCurrExpPrie = 0.0f;
	m_fNeedExpPrie = 0.0f;	

	m_iLevelOccu = 0;
	m_fCurrExpOccu = 0.0f;
	m_fNeedExpOccu = 0.0f;	
}

PlayerCharacter::~PlayerCharacter(void)
{
	if(m_pModel)
		delete m_pModel;
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

#include "StdAfx.h"
#include "PlayerCharacter.h"
#include "Common/Graphics/kpgModel.h"
#include "Common/Graphics/kpgGeometryInstance.h"

PlayerCharacter::PlayerCharacter(void)
{
	m_pModel = new kpgModel();
	if( !m_pModel->Load("Assets\\Player\\PlayerCard.dae") )
	{
		delete m_pModel;
		m_pModel = 0;
	}

	m_vMoveTarget = GetLocation();
	m_fHeightOffset = m_vMoveTarget.GetY();
}

PlayerCharacter::~PlayerCharacter(void)
{
	if( m_pModel )
		delete m_pModel;
}

kpuVector PlayerCharacter::GetLocation()
{
	if( m_pModel )
	{
		return m_pModel->GetInstance(0)->GetMatrix().GetD();
	}
	else
	{
		return kpuVector();
	}
}

void PlayerCharacter::SetLocation(const kpuVector& vNewLoc)
{
	if( m_pModel )
	{
		m_pModel->GetInstance(0)->SetPosition(vNewLoc.GetX(), vNewLoc.GetY(), vNewLoc.GetZ());
	}
}

void PlayerCharacter::Update()
{
	if( !m_pModel )
		return;

	// Update player position
	kpuVector vPlayerToTarget = m_vMoveTarget - GetLocation();
	if( vPlayerToTarget.Dot(vPlayerToTarget) > 1 )
	{
		SetLocation(m_vMoveTarget);
	}
}

void PlayerCharacter::Draw(kpgRenderer* pRenderer)
{
	if( m_pModel )
		m_pModel->Draw(pRenderer);
}

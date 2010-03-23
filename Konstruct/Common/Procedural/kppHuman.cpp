#include "StdAfx.h"
#include "Common\Procedural\kppHuman.h"
#include "Common\Procedural\kppBox.h"
#include "Common\Graphics\kpgGeometryInstance.h"
#include "Common\Graphics\kpgRenderer.h"

kppHuman::kppHuman(float fHeight, float fShoulderWidth)
{
	m_fHeight = fHeight;
	m_fShoulderWidth = fShoulderWidth;
	m_mLocalToWorld.Identity();
}

kppHuman::~kppHuman(void)
{
}

void kppHuman::Build()
{
	// Calculate some sizes
	float fLegLength = m_fHeight * 0.5f;
	float fTorsoHeight = m_fHeight * 0.4f;
	float fHeadHeight = m_fHeight * 0.1f;
	float fArmLength = m_fHeight * 0.5f;

	float fArmWidth = m_fShoulderWidth * 0.05f;
	float fTorsoWidth = m_fShoulderWidth * 0.9f;
	float fTorsoDepth = fTorsoWidth * 0.67f;
	float fLegWidth = fTorsoWidth * 0.5f;
	float fHeadWidth = fTorsoWidth * 0.6f;
	float fHeadDepth = fTorsoDepth * 1.15f;

	float fFootHeight = fLegLength * 0.15f;
	float fFootDepth = fTorsoDepth * 1.25f;
	float fUpperLegHeight = fLegLength * 0.45f;
	float fLowerLegHeight = fLegLength * 0.40f;
	
	float fHandHeight = fArmLength * 0.15f;
	float fUpperArmHeight = fArmLength * 0.40f;
	float fLowerArmHeight = fArmLength * 0.45f;

	// Setup the boxes
	m_pBodyBoxes[eBP_LeftFoot]		= new kppBox(fLegWidth,		fFootHeight,		fFootDepth		);
	m_pBodyBoxes[eBP_LeftLowerLeg]	= new kppBox(fTorsoDepth,	fLowerLegHeight,	fLegWidth		);
	m_pBodyBoxes[eBP_LeftUpperLeg]	= new kppBox(fTorsoDepth,	fUpperLegHeight,	fLegWidth		);
	m_pBodyBoxes[eBP_Torso]			= new kppBox(fTorsoDepth,	fTorsoHeight,		fTorsoWidth	);
	m_pBodyBoxes[eBP_Head]			= new kppBox(fHeadDepth,	fHeadHeight,		fHeadWidth		);
	m_pBodyBoxes[eBP_LeftUpperArm]	= new kppBox(fTorsoDepth,	fUpperArmHeight,	fArmWidth		);
	m_pBodyBoxes[eBP_LeftLowerArm]	= new kppBox(fTorsoDepth,	fLowerArmHeight,	fArmWidth		);
	m_pBodyBoxes[eBP_LeftHand]		= new kppBox(fTorsoDepth,	fHandHeight,		fArmWidth);
	
	// Build the geometry
	for( int i = 0; i < eBP_RightUpperArm; i++ )
	{
		m_pBodyBoxes[i]->Build();
	}

	// Create instances
	for( int i = 0; i < eBP_Last; i++ )
	{
		int iBoxIndex = i;
		if( iBoxIndex > eBP_LeftFoot )
			iBoxIndex -= 6;

		m_pBodyInsts[i] = new kpgGeometryInstance(m_pBodyBoxes[iBoxIndex]);
	}

	// Place all the instances
	m_pBodyInsts[eBP_LeftFoot]->SetPosition(	fLegWidth * 0.5f,	fFootHeight * 0.5f,		fFootDepth * 0.5f);
	m_pBodyInsts[eBP_RightFoot]->SetPosition(	fLegWidth * -0.5f,	fFootHeight * 0.5f,		fFootDepth * 0.5f);
	
	m_pBodyInsts[eBP_LeftLowerLeg]->SetPosition(	fLegWidth * 0.5f,	fFootHeight + (fLowerLegHeight * 0.5f), fTorsoDepth * 0.5f);
	m_pBodyInsts[eBP_RightLowerLeg]->SetPosition(	fLegWidth * -0.5f,	fFootHeight + (fLowerLegHeight * 0.5f), fTorsoDepth * 0.5f);

	m_pBodyInsts[eBP_LeftUpperLeg]->SetPosition(	fLegWidth * 0.5f,	fFootHeight + fLowerLegHeight + (fUpperLegHeight * 0.5f), fTorsoDepth * 0.5f);
	m_pBodyInsts[eBP_RightUpperLeg]->SetPosition(	fLegWidth * -0.5f,	fFootHeight + fLowerLegHeight + (fUpperLegHeight * 0.5f), fTorsoDepth * 0.5f);

	m_pBodyInsts[eBP_Torso]->SetPosition( 0.0f, fLegLength + (fTorsoHeight * 0.5f), fTorsoDepth * 0.5f);
	
	m_pBodyInsts[eBP_Head]->SetPosition( 0.0f, fLegLength + fTorsoHeight + (fHeadHeight * 0.5f),  fHeadDepth * 0.5f);
	
	m_pBodyInsts[eBP_LeftUpperArm]->SetPosition( (fTorsoWidth * 0.5f) + (fArmWidth * 0.5f),		fLegLength + fTorsoHeight - (fUpperArmHeight * 0.5f), fTorsoDepth * 0.5f);	
	m_pBodyInsts[eBP_RightUpperArm]->SetPosition( (fTorsoWidth * -0.5f) - (fArmWidth * 0.5f),	fLegLength + fTorsoHeight - (fUpperArmHeight * 0.5f), fTorsoDepth * 0.5f);

	m_pBodyInsts[eBP_LeftLowerArm]->SetPosition( (fTorsoWidth * 0.5f) + (fArmWidth * 0.5f),		fLegLength + fTorsoHeight - fUpperArmHeight - (fLowerArmHeight * 0.5f), fTorsoDepth * 0.5f);	
	m_pBodyInsts[eBP_RightLowerArm]->SetPosition( (fTorsoWidth * -0.5f) - (fArmWidth * 0.5f),	fLegLength + fTorsoHeight - fUpperArmHeight - (fLowerArmHeight * 0.5f), fTorsoDepth * 0.5f);

	m_pBodyInsts[eBP_LeftHand]->SetPosition( (fTorsoWidth * 0.5f) + (fArmWidth * 0.5f),		fLegLength + fTorsoHeight - fUpperArmHeight - fLowerArmHeight - (fHandHeight * 0.5f), fTorsoDepth * 0.5f);	
	m_pBodyInsts[eBP_RightHand]->SetPosition( (fTorsoWidth * -0.5f) - (fArmWidth * 0.5f),	fLegLength + fTorsoHeight - fUpperArmHeight - fLowerArmHeight - (fHandHeight * 0.5f), fTorsoDepth * 0.5f);
}

void kppHuman::Draw()
{
	kpgRenderer* pRenderer = kpgRenderer::GetInstance();

	for( int i = 0; i < eBP_Last; i++ )
	{
		//pRenderer->DrawInstancedGeometry(m_pBodyInsts[i], &m_mLocalToWorld);
	}

	pRenderer->DrawInstancedGeometry(m_pBodyInsts[eBP_LeftFoot], &m_mLocalToWorld);
	pRenderer->DrawInstancedGeometry(m_pBodyInsts[eBP_LeftLowerLeg], &m_mLocalToWorld);
}
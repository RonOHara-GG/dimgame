#include "StdAfx.h"
#include "GameState_Default.h"
#include "Texture_FloorPlating.h"
#include "Common\Procedural\kppBox.h"
#include "Common\Procedural\kppPlane.h"
#include "Common\Procedural\kppTerrain.h"
#include "Common\Procedural\kppHuman.h"
#include "Common\Graphics\kpgGeometryInstance.h"
#include "Common\Graphics\kpgRenderer.h"
#include "Common\Graphics\kpgLight.h"
#include "Common\Graphics\kpgShader.h"

GameState_Default::GameState_Default(void)
{
	m_pSunLight = new kpgLight(kpgLight::eLT_Directional);

	m_pMyPlane = new kppPlane(1000.0f, 1000.0f, kpuv_OneY);
	m_pMyPlane->Build();

	m_pMyTerrain = new kppTerrain(200, 10.0f, 200, 10.0f);
	m_pMyTerrain->Build();

	m_pMyBox = new kppBox();
	m_pMyBox->SetWidth(3.0f);
	m_pMyBox->SetHeight(3.0f);
	m_pMyBox->SetDepth(3.0f);
	m_pMyBox->Build();

	for( int i = 0; i < 4; i++ )
	{
		m_pBoxes[i] = new kpgGeometryInstance(m_pMyBox);
	}

	m_pBoxes[0]->SetPosition(0.0f, 0.0f, 0.0f);
	m_pBoxes[1]->SetPosition(0.0f, 4.0f, -7.0f);
	m_pBoxes[2]->SetPosition(6.0f, -3.0f, 4.0f);
	m_pBoxes[3]->SetPosition(3.0f, 0.0f, 5.0f);

	m_pGroundPlane = new kpgGeometryInstance(m_pMyPlane);
	m_pGroundPlane->SetPosition(0.0f, 0.0f, 0.0f);

	m_pTerrain = new kpgGeometryInstance(m_pMyTerrain);
	m_pTerrain->SetPosition(-1000.0f, 0.0f, -1000.0f);

	m_pMyFloorTexture = new Texture_FloorPlating(256, 256);
	m_pMyFloorTexture->Build();

	m_pMainCharacter = new kppHuman(1.8f, 0.4f);
	m_pMainCharacter->Build();
	m_pMainCharacter->SetPosition(0.0f, 0.0f, 0.0f);
}

GameState_Default::~GameState_Default(void)
{
	delete m_pGroundPlane;
	for( int i = 0; i < 4; i++ )
		delete m_pBoxes[i];
	
	delete m_pMyBox;
	delete m_pMyPlane;

	delete m_pSunLight;
}

void GameState_Default::Update()
{
	m_SunAndMoon.Update();
	m_pSunLight->SetDirection(m_SunAndMoon.GetLightDirection());
	m_pSunLight->SetColor(m_SunAndMoon.GetLightColor());
}

void GameState_Default::Draw()
{
	kpgRenderer* pRenderer = kpgRenderer::GetInstance();
	
	pRenderer->SetAmbientLightColor(m_SunAndMoon.GetAmbientColor());
	pRenderer->SetLight(0, m_pSunLight);
	//pRenderer->SetAmbientLightColor(kpuVector(1.0f, 1.0f, 1.0f, 1.0f));

	for( int i = 0; i < 4; i++ )
	{
		//pRenderer->DrawInstancedGeometry(m_pBoxes[i]);
	}

	m_pMyBox->GetShader()->SetDefaultTexture(m_pMyFloorTexture);
	pRenderer->DrawInstancedGeometry(m_pBoxes[0]);

	//m_pMyPlane->GetShader()->SetDefaultTexture(m_pMyFloorTexture);
	//pRenderer->DrawInstancedGeometry(m_pGroundPlane);

	//pRenderer->DrawInstancedGeometry(m_pTerrain);

	//m_pMainCharacter->Draw();
}
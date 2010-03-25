#include "StdAfx.h"
#include "GameState_Default.h"
#include "LevelManager.h"
#include "Level.h"
#include "PlayerCharacter.h"
#include "Common\Procedural\kppBox.h"
#include "Common\Procedural\kppPlane.h"
#include "Common\Procedural\kppTerrain.h"
#include "Common\Procedural\kppHuman.h"
#include "Common\Graphics\kpgGeometryInstance.h"
#include "Common\Graphics\kpgRenderer.h"
#include "Common\Graphics\kpgLight.h"
#include "Common\Graphics\kpgShader.h"
#include "Common\Utility\kpuCameraController.h"

extern kpuCameraController*	g_pCamera;

GameState_Default::GameState_Default(void)
{
	LevelManager* pLevelManager = LevelManager::GetInstance();
	m_pCurrentLevel = pLevelManager->LoadLevel(eLID_SpaceStation);

	m_pPlayer = new PlayerCharacter();
}

GameState_Default::~GameState_Default(void)
{
	delete m_pCurrentLevel;
	delete m_pPlayer;
}

void GameState_Default::MouseUpdate(int X, int Y)
{
	// Find projected ground point for the screen coordinates
	kpgRenderer* pRenderer = kpgRenderer::GetInstance();
	kpuMatrix mIProj = pRenderer->GetProjectionMatrix();
	kpuMatrix mIView = pRenderer->GetViewMatrix();	
	kpuVector vRayDir;
	vRayDir.SetX(-(((2.0f * (float)X) / pRenderer->GetScreenWidth()) - 1) / mIProj.GetA().GetX());
	vRayDir.SetY((((2.0f * (float)Y) / pRenderer->GetScreenHeight()) - 1) / mIProj.GetB().GetY());
	vRayDir.SetZ(1.0f);
	vRayDir.SetW(0.0f);
	mIView.Invert();
	vRayDir *= mIView;

	// Project the ray onto the ground plane to find the ground point
	kpuVector vRayOrigin = mIView.GetD();
	float fT = -vRayOrigin.GetY() / vRayDir.GetY();
	kpuVector vGroundPoint = vRayOrigin + (vRayDir * fT);
	
	// Update the players move target to the new ground point
	m_pPlayer->SetMoveTarget(m_pCurrentLevel->GetGrid()->GetTile(vGroundPoint)->GetCenter());
}

void GameState_Default::Update(float fGameTime)
{
	if( m_pCurrentLevel )
		m_pCurrentLevel->Update();

	if( m_pPlayer )
	{
		m_pPlayer->Update(fGameTime);

		g_pCamera->SetLookAt(m_pPlayer->GetLocation());
	}
}

void GameState_Default::Draw()
{
	kpgRenderer* pRenderer = kpgRenderer::GetInstance();
	if( m_pCurrentLevel )
		m_pCurrentLevel->Draw(pRenderer);

	if( m_pPlayer )
		m_pPlayer->Draw(pRenderer);
}
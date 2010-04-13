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
#include "Common\Utility\kpuVector.h"
#include "Grid.h"
#include "Enemy.h"

extern kpuCameraController*	g_pCamera;

GameState_Default::GameState_Default(void)
{
	LevelManager* pLevelManager = LevelManager::GetInstance();
	m_pCurrentLevel = pLevelManager->LoadLevel(eLID_SpaceStation);

	m_pPlayer = new PlayerCharacter();
	m_paEnemies = new kpuArrayList<Enemy*>();
	
	m_pCurrentLevel->LoadEnemyList(m_paEnemies);

}

GameState_Default::~GameState_Default(void)
{
	delete m_pCurrentLevel;
	delete m_pPlayer;
	
	if(m_paEnemies)
		delete m_paEnemies;
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
	int iTile = m_pCurrentLevel->GetGrid()->GetTileAtLocation(vGroundPoint);
	m_pPlayer->SetMoveTarget(iTile);

	//If target tile contains an enemy try and attack
	Actor* pTarget = m_pCurrentLevel->GetGrid()->GetActor(iTile);

	if(pTarget)
	{
		m_pPlayer->SetTarget(pTarget);
		m_pPlayer->UseDefaultAttack(pTarget, m_pCurrentLevel->GetGrid());
	}
	
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

	if(m_paEnemies)
	{
		for(int i = 0; i < m_paEnemies->Count(); i++)
		{
			Enemy* pEnemy = (*m_paEnemies)[i];

			if(!pEnemy->Update(fGameTime))
			{
				m_paEnemies->Remove(pEnemy);
				delete pEnemy;
				pEnemy = 0;
			}
		}
	}
}

void GameState_Default::Draw()
{
	kpgRenderer* pRenderer = kpgRenderer::GetInstance();
	if( m_pCurrentLevel )
		m_pCurrentLevel->Draw(pRenderer);

	if( m_pPlayer )
		m_pPlayer->Draw(pRenderer);

	if(m_paEnemies)
	{
		for(int i = 0; i < m_paEnemies->Count(); i++)
		{
			Enemy* pEnemy = (*m_paEnemies)[i];

			pEnemy->Draw(pRenderer);
		}
	}
}

void GameState_Default::AddEnemy(Enemy* pEnemy)
{
	if(m_paEnemies)
		m_paEnemies->Add(pEnemy); 
}
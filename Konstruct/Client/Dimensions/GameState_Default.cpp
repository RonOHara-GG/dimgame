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
#include "Common/Utility/kpuQuadTree.h"
#include "Common\Input\kpiInputManager.h"

extern kpuCameraController*	g_pCamera;

GameState_Default::GameState_Default(void)
{
	LevelManager* pLevelManager = LevelManager::GetInstance();
	//m_pCurrentLevel = pLevelManager->LoadLevel(eLID_SpaceStation);
	m_pCurrentLevel = pLevelManager->LoadLevel(eLID_Bastarak);
	m_pPlayer = new PlayerCharacter();
	m_pCurrentLevel->GetQuadTree()->Add(m_pPlayer);
	m_paActors = new kpuArrayList<Actor*>();

	m_paActors->Add(m_pPlayer);
	m_pCurrentLevel->GenerateEnemies(m_paActors); 

	

}

GameState_Default::~GameState_Default(void)
{
	delete m_pCurrentLevel;
	delete m_pPlayer;
	
	if(m_paActors)
	{
		for(int i = 0; i < m_paActors->Count(); i++)
		{
			Actor* pActor = (*m_paActors)[i];

			m_paActors->RemoveAt(i);
			delete pActor;
		}

		delete m_paActors;

	}
		
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

	if( m_pCurrentLevel->GetGrid()->TileWalkable(iTile) )
	{	
		//If target tile contains an enemy try and attack
		Actor* pTarget = m_pCurrentLevel->GetGrid()->GetActor(iTile);

		if( pTarget && pTarget->HasFlag(ATTACKABLE) && m_pPlayer->IsInRange(pTarget, m_pPlayer->GetRange()) )
		{
			m_pPlayer->SetTarget(pTarget);
			m_pPlayer->UseDefaultAttack(pTarget, m_pCurrentLevel->GetGrid());
		}
		else
			m_pPlayer->SetMoveTarget(iTile);	
	}
	
}

void GameState_Default::Update(float fGameTime)
{
	if( m_pCurrentLevel )
		m_pCurrentLevel->Update();

	if( m_pPlayer )
	{	
		g_pCamera->SetLookAt(m_pPlayer->GetLocation());
	}

	if(m_paActors)
	{
		for(int i = 0; i < m_paActors->Count(); i++)
		{
			Actor* pActor = (*m_paActors)[i];

			if(!pActor->Update(fGameTime))
			{
				m_paActors->Remove(pActor);
				pActor->GetCurrentNode()->Remove(pActor);
				m_pCurrentLevel->GetGrid()->RemoveActor(pActor);
				delete pActor;
				pActor= 0;
			}
		}
	}
}

void GameState_Default::Draw()
{
	kpgRenderer* pRenderer = kpgRenderer::GetInstance();
	if( m_pCurrentLevel )
		m_pCurrentLevel->Draw(pRenderer);

	/*if( m_pPlayer )
		m_pPlayer->Draw(pRenderer);*/

	if(m_paActors)
	{
		for(int i = 0; i < m_paActors->Count(); i++)
		{
			Actor* pActor = (*m_paActors)[i];

			pActor->Draw(pRenderer);
		}
	}
}

void GameState_Default::AddActor(Actor* pActor)
{
	if(m_paActors)
		m_paActors->Add(pActor); 
}

bool GameState_Default::HandleInputEvent(eInputEventType type, u32 button)
{
	switch(type)
	{
	case eIET_MouseDrag:
		{	
			POINT ptMousePos = g_pInputManager->GetMouseLoc();
			MouseUpdate(ptMousePos.x, ptMousePos.y );
			break;
		}
	case eIET_ButtonDown:
		{		
			break;
		}
	case eIET_ButtonUp:
		{
			switch(button)
			{
			case KPIM_BUTTON_0:
				{
					POINT ptMousePos = g_pInputManager->GetMouseLoc();
					MouseUpdate(ptMousePos.x, ptMousePos.y );
					break;
				}
			}
			break;
		}
	case eIET_ButtonClick:
		{
			switch(button)
			{
			case KPIM_BUTTON_0:
				{
					POINT ptMousePos = g_pInputManager->GetMouseLoc();
					MouseUpdate(ptMousePos.x, ptMousePos.y );
					break;
				}
			}
			break;
		}
	case eIET_ButtonDoubleClick:
		{
			switch(button)
			{
			case KPIM_BUTTON_0:
				{
					POINT ptMousePos = g_pInputManager->GetMouseLoc();
					MouseUpdate(ptMousePos.x, ptMousePos.y );
					break;
				}
			}
			break;
		}
	}
	return false;
}
#include "StdAfx.h"
#include "GameState_GamePlay.h"
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
#include "Common\Graphics\kpgUIManager.h"
#include "Common\Utility\kpuCameraController.h"
#include "Common\Utility\kpuThreeQuarterTopDownCamera.h"
#include "Common\Utility\kpuVector.h"
#include "Grid.h"
#include "Enemy.h"
#include "Common/Utility/kpuQuadTree.h"
#include "Common\Input\kpiInputManager.h"

GameState_GamePlay::GameState_GamePlay(void)
{
	// Create the UI manager & load the UI for this game state
	m_pUIManager = new kpgUIManager();

	// Setup basic camera info
	kpuVector vLocation(12.0f, 18.0f, 12.0f, 0.0f);
	kpuVector vLookAt(0.0f, 0.0f, 0.0f, 0.0f);
	m_pCamera = new kpuThreeQuarterTopDownCamera(vLocation, vLookAt, kpuv_OneY);

	// Setup render matrices
	kpgRenderer* pRenderer = kpgRenderer::GetInstance();
	m_mProjection.Perspective(45.0f, pRenderer->GetScreenWidth() / pRenderer->GetScreenHeight(), 0.001f, 10000.0f);

	pRenderer->SetProjectionMatrix(m_mProjection);
	pRenderer->SetViewMatrix(m_pCamera->GetViewMatrix());

	// setup render state
	pRenderer->SetCullMode(kpgRenderer::eCM_None);
    pRenderer->GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	
	// for now, setup a temp light
	kpgLight* pDummyLight = new kpgLight(kpgLight::eLT_Directional);
	kpuVector vLightDir(-1, -1, 0, 0);
	vLightDir.Normalize();
	pDummyLight->SetDirection(vLightDir);
	pDummyLight->SetColor(kpuVector(0.0f, 0.0f, 0.0f, 0.75f));
	pRenderer->SetLight(0, pDummyLight);
}

GameState_GamePlay::~GameState_GamePlay(void)
{
	//delete m_pCurrentLevel;
		
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

void GameState_GamePlay::MouseUpdate(int X, int Y)
{
	kpuVector vGroundPoint((float)X, (float)Y, 0.0f, 0.0f);

	ScreenCordsToGameCords(vGroundPoint);	

	//set heading to mouse
	m_pPlayer->SetHeading(kpuVector::Normalize(vGroundPoint - m_pPlayer->GetLocation()));
	
	// Update the players move target to the new ground point
	int iTile = m_pCurrentLevel->GetGrid()->GetTileAtLocation(vGroundPoint);

	if( m_pCurrentLevel->GetGrid()->TileWalkable(iTile) )
	{	
		//If target tile contains an actor get it and process event
		Actor* pTarget = m_pCurrentLevel->GetGrid()->GetActor(iTile);
		if( pTarget )
		{
			if( pTarget->HasFlag(ATTACKABLE) && m_pPlayer->IsInRange(pTarget, m_pPlayer->GetRange()) )
			{
				//attack it
				m_pPlayer->SetTarget(pTarget);
				m_pPlayer->UseDefaultAttack(pTarget, m_pCurrentLevel->GetGrid());

			}
			else if( pTarget->HasFlag(NPC) && pTarget->IsInRange(m_pPlayer, pTarget->GetRange()) )
			{
				Npc* pNpc = (Npc*)pTarget;

				pNpc->Interact(m_pPlayer);
			}
			else
				m_pPlayer->SetMoveTarget(iTile);
		}
		else
			m_pPlayer->SetMoveTarget(iTile);
		
	}
	
}

void GameState_GamePlay::ScreenCordsToGameCords(kpuVector& vCords)
{
	// Find projected ground point for the screen coordinates
	kpgRenderer* pRenderer = kpgRenderer::GetInstance();
	kpuMatrix mIProj = m_mProjection;
	kpuMatrix mIView = m_pCamera->GetViewMatrix();	
	kpuVector vRayDir;
	vRayDir.SetX(-(((2.0f * vCords.GetX()) / pRenderer->GetScreenWidth()) - 1) / mIProj.GetA().GetX());
	vRayDir.SetY((((2.0f * vCords.GetY()) / pRenderer->GetScreenHeight()) - 1) / mIProj.GetB().GetY());
	vRayDir.SetZ(1.0f);
	vRayDir.SetW(0.0f);
	mIView.Invert();
	vRayDir *= mIView;

	// Project the ray onto the ground plane to find the ground point
	kpuVector vRayOrigin = mIView.GetD();
	float fT = -vRayOrigin.GetY() / vRayDir.GetY();
	vCords = vRayOrigin + (vRayDir * fT);
}

void GameState_GamePlay::Update(float fDeltaTime)
{
	// Update the UI
	m_pUIManager->Update();

	if( m_pPlayer )
	{
		m_pCamera->SetLookAt(m_pPlayer->GetLocation());
	}

	m_pCamera->Update();

	if( m_pCurrentLevel )
		m_pCurrentLevel->Update(fDeltaTime);

	if(m_paActors)
	{
		for(int i = 0; i < m_paActors->Count(); i++)
		{
			Actor* pActor = (*m_paActors)[i];

			if(!pActor->Update(fDeltaTime))
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

void GameState_GamePlay::Draw()
{
	kpgRenderer* pRenderer = kpgRenderer::GetInstance();

	pRenderer->SetProjectionMatrix(m_mProjection);
	pRenderer->SetViewMatrix(m_pCamera->GetViewMatrix());
	pRenderer->SetAmbientLightColor(kpuVector(0.75f, 0.75f, 0.75f, 1.0f));	

	if( m_pCurrentLevel )
		m_pCurrentLevel->Draw(pRenderer);

	if(m_paActors)
	{
		for(int i = 0; i < m_paActors->Count(); i++)
		{
			Actor* pActor = (*m_paActors)[i];

			pActor->Draw(pRenderer);
		}
	}

	// Draw UI
	m_pUIManager->Draw(pRenderer);
}

void GameState_GamePlay::AddActor(Actor* pActor)
{
	if(m_paActors)
		m_paActors->Add(pActor); 
}

bool GameState_GamePlay::HandleInputEvent(eInputEventType type, u32 button)
{
	if( m_pUIManager && m_pUIManager->HandleInputEvent(type, button) != IE_NOT_HANDLED )
		return true;

	bool bHandled = false;
	kpPoint ptMousePos = g_pInputManager->GetMouseLoc();
	switch(type)
	{		
		case eIET_ButtonUp:
			switch(button)
			{
				case KPIM_BUTTON_0:
					MouseUpdate(ptMousePos.m_iX, ptMousePos.m_iY );
					bHandled = true;
					break;
			}
			break;
	}
	return bHandled;
}
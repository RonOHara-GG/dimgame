// Dimensions.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Dimensions.h"
#include "LevelManager.h"
#include "GameState.h"
#include "GameState_Default.h"
#include "Common/Utility/kpuCameraController.h"
#include "Common/Utility/kpuThreeQuarterTopDownCamera.h"
#include "Common/Utility/kpuFileManager.h"
#include "Common/Input/kpiInputManager.h"
#include "Common/Graphics/kpgRenderer.h"
#include "Common/Graphics/kpgUIManager.h"
#include "Common/Graphics/kpgTexture.h"
#include "Common/Graphics/kpgGeometryInstance.h"
#include "Common/Graphics/kpgLight.h"
#include "Common/Procedural/kppPlane.h"

#include <math.h>
#include <d3dx9math.h>

extern HWND CreateRenderWindow(HINSTANCE hInstance);
extern void UpdateRenderWindow(HWND hWnd);
extern void CloseRenderWindow(HWND hWnd);

kpuCameraController*	g_pCamera = 0;
bool					g_bExitGame = false;

void UpdateGame();
void DrawGame();

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	// Create the render window
	HWND hWnd = CreateRenderWindow(hInstance);
	if( !hWnd )
	{
		MessageBox(NULL, TEXT("Failed to create the render window!"), TEXT("Error"), MB_ICONSTOP);
		return -1;
	}

	// Set the file path
	char szCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurrentDir);
	kpuFileManager::SetRootPath(szCurrentDir);

	// Create the input manager
	kpiInputManager* pInputManager = new kpiInputManager();

	// Create the renderer
	kpgRenderer* pRenderer = kpgRenderer::GetInstance();
	pRenderer->Create(hWnd);

	// Create the UI manager
	kpgUIManager* pUIManager = new kpgUIManager();

	kpuVector vLocation(12.0f, 18.0f, 12.0f, 0.0f);
	kpuVector vLookAt(0.0f, 0.0f, 0.0f, 0.0f);
	g_pCamera = new kpuThreeQuarterTopDownCamera(vLocation, vLookAt, kpuv_OneY);

	kpuMatrix mProjection, mView, mWorld;
	mProjection.Perspective(45.0f, pRenderer->GetScreenWidth() / pRenderer->GetScreenHeight(), 0.001f, 10000.0f);
	mWorld.Identity();

	pRenderer->SetProjectionMatrix(mProjection);
	pRenderer->SetViewMatrix(g_pCamera->GetViewMatrix());
	pRenderer->SetWorldMatrix(mWorld);

	pRenderer->GetDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
    pRenderer->GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );


	kppPlane* m_pMyPlane = new kppPlane(10.0f, 20.0f, kpuv_OneY);
	m_pMyPlane->Build();

	kpgGeometryInstance* m_pGroundPlane = new kpgGeometryInstance(m_pMyPlane);
	m_pGroundPlane->SetPosition(0.0f, 0.0f, 0.0f);
	
	kpgLight* m_pDummyLight = new kpgLight(kpgLight::eLT_Directional);
	kpuVector vLightDir(-1, -1, 0, 0);
	vLightDir.Normalize();
	m_pDummyLight->SetDirection(vLightDir);
	m_pDummyLight->SetColor(kpuVector(1.0f, 1.0f, 1.0f, 1.0f));
	
	// Process until exit
	while( pInputManager->Update() )
	{
		// Update the window
		UpdateRenderWindow(hWnd);
		if( g_bExitGame )
			break;

		UpdateGame();
		g_pCamera->Update();

		// Update the UI
		pUIManager->Update();

		// Begin the frame
		pRenderer->BeginFrame();
		pRenderer->SetViewMatrix(g_pCamera->GetViewMatrix());

		pRenderer->SetAmbientLightColor(kpuVector(0.25f, 0.25f, 0.25f, 1.0f));
		pRenderer->SetLight(0, m_pDummyLight);

		//pRenderer->DrawInstancedGeometry(m_pGroundPlane);

		DrawGame();

		// Draw UI
		pUIManager->Draw(pRenderer);

		// End the frame
		pRenderer->EndFrame();

		// Let some other threads have some time
		Sleep(1);
	}

	LevelManager::Shutdown();

	// Destroy the UI manager
	delete pUIManager;

	// Destroy the renderer
	delete pRenderer;

	// Destroy the input manager
	delete pInputManager;

	// Kill the window
	CloseRenderWindow(hWnd);

	return 0;
}

GameState* g_pGameState = 0;

void UpdateGame()
{
	if( !g_pGameState )
	{
		// GameState hasnt been created yet, make the default
		g_pGameState = new GameState_Default();
	}

	g_pGameState->Update();
}

void DrawGame()
{
	if( g_pGameState )
	{
		g_pGameState->Draw();
	}
}
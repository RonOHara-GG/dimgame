// SpaceProc.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SpaceProc.h"
#include "GameState.h"
#include "GameState_Default.h"
#include "CameraController.h"
#include "ThreeQuarterTopDownCamera.h"
#include "Common/Input/kpiInputManager.h"
#include "Common/Graphics/kpgRenderer.h"
#include "Common/Graphics/kpgUIManager.h"
#include "Common/Graphics/kpgTexture.h"

extern HWND CreateRenderWindow(HINSTANCE hInstance);
extern void UpdateRenderWindow(HWND hWnd);
extern void CloseRenderWindow(HWND hWnd);

CameraController*	g_pCamera = 0;

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
		MessageBox(NULL, "Failed to create the render window!", "Error", MB_ICONSTOP);
		return -1;
	}

	// Create the input manager
	kpiInputManager* pInputManager = new kpiInputManager();

	// Create the renderer
	kpgRenderer* pRenderer = kpgRenderer::GetInstance();
	pRenderer->Create(hWnd);

	// Create the UI manager
	kpgUIManager* pUIManager = new kpgUIManager();

	kpuMatrix mProjection, mView, mWorld;
	mProjection.Perspective(45.0f, pRenderer->GetScreenWidth() / pRenderer->GetScreenHeight(), 0.001f, 1000.0f);
	//mProjection.Orthographic(pRenderer->GetScreenWidth(), pRenderer->GetScreenHeight(), 0.001f, 1000.0f);
	mView.LookAt(kpuVector(0, 15, -15.0f, 0.0f), kpuVector(0.0f, 0.0f, 0.0f, 0.0f), kpuVector(0.0f, 1.0f, 0.0f, 0.0f));
	mWorld.Identity();

	pRenderer->SetProjectionMatrix(mProjection);
	pRenderer->SetViewMatrix(mView);	
	pRenderer->SetWorldMatrix(mWorld);

	pRenderer->GetDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
    pRenderer->GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );

	g_pCamera = new ThreeQuarterTopDownCamera();
	
	// Process until exit
	while( pInputManager->Update() )
	{
		// Update the window
		UpdateRenderWindow(hWnd);

		g_pCamera->Update();
		UpdateGame();

		// Update the UI
		pUIManager->Update();

		// Begin the frame
		pRenderer->BeginFrame();
		pRenderer->SetViewMatrix(g_pCamera->GetViewMatrix());

		DrawGame();

		// Draw UI
		pUIManager->Draw(pRenderer);

		// End the frame
		pRenderer->EndFrame();

		// Let some other threads have some time
		Sleep(1);
	}

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

GameState*	g_pGameState = 0;

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
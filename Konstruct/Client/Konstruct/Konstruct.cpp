// Konstruct.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Konstruct.h"
#include "Common/Input/kpiInputManager.h"
#include "Common/Graphics/kpgRenderer.h"
#include "Common/Graphics/kpgUIManager.h"
#include "Common/Graphics/kpgTexture.h"

extern HWND CreateRenderWindow(HINSTANCE hInstance);
extern void UpdateRenderWindow(HWND hWnd);
extern void CloseRenderWindow(HWND hWnd);

// Our custom FVF, which describes our custom vertex structure
//#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL | D3DFVF_TEX0|D3DFVF_DIFFUSE)
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_TEX0|D3DFVF_DIFFUSE)

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
	//mProjection.Perspective(45.0f, pRenderer->GetScreenWidth() / pRenderer->GetScreenHeight(), 0.001f, 1000.0f);
	mProjection.Orthographic(pRenderer->GetScreenWidth(), pRenderer->GetScreenHeight(), 0.001f, 1000.0f);
	mView.LookAt(kpuVector(0, 0, -3.0f, 0.0f), kpuVector(0.0f, 0.0f, 0.0f, 0.0f), kpuVector(0.0f, 1.0f, 0.0f, 0.0f));
	mWorld.Identity();

	pRenderer->SetProjectionMatrix(mProjection);
	pRenderer->SetViewMatrix(mView);	
	pRenderer->SetWorldMatrix(mWorld);

	pRenderer->GetDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
    pRenderer->GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );

	kpgTexture* pTexture = new kpgTexture();
	pTexture->Load("C:\\projects\\Konstruct\\Client\\Konstruct\\ScreenLayouts\\System\\Images\\DefaultBackground.dds");
	
	// Process until exit
	while( pInputManager->Update() )
	{
		// Update the window
		UpdateRenderWindow(hWnd);

		// Update the UI
		pUIManager->Update();

		// Begin the frame
		pRenderer->BeginFrame();

		// Draw UI
		pUIManager->Draw(pRenderer);

		pRenderer->DrawQuad2D(kpRect(10.0f, 50.0f, 10.0f, 50.0f), pTexture);
		//pRenderer->DrawQuad2D(kpRect(0.0f, 100.0f, 100.0f, 0.0f), pTexture);

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

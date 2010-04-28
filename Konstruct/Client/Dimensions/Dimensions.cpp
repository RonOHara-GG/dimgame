// Dimensions.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Dimensions.h"
#include "LevelManager.h"
#include "GameState.h"
#include "GameState_Default.h"
#include "LoadStructures.h"
#include "External/tinyxml/tinyxml.h"
#include "Common/Utility/kpuFixedArray.h"
#include "Common/Graphics/kpgModel.h"
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

//Timing varibles
__int64					g_iStart, g_iEnd, g_iFrequency;
float					g_fElasped;

//Global game data
extern kpuFixedArray<EnemyLoadStructure*>* g_paEnemyTypes;

void UpdateGame();
void DrawGame();
void LoadEnemyList();
void LoadEnemyType(const char* pszFile);

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

	//Initalize the timeing varibles
	QueryPerformanceFrequency((LARGE_INTEGER*)&g_iFrequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&g_iStart);
	g_fElasped = 0.0f;

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
	m_pDummyLight->SetColor(kpuVector(0.0f, 0.0f, 0.0f, 0.75f));

	//Load enemies
	LoadEnemyList();

	// Process until exit
	while( pInputManager->Update() )
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&g_iEnd);

		g_fElasped = (g_iEnd - g_iStart) / (float)g_iFrequency;

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

		pRenderer->SetAmbientLightColor(kpuVector(0.75f, 0.75f, 0.75f, 1.0f));
		pRenderer->SetLight(0, m_pDummyLight);
		//pRenderer->SetLight(0, m_pPointLight);

		//pRenderer->DrawInstancedGeometry(m_pGroundPlane);

		DrawGame();

		// Draw UI
		pUIManager->Draw(pRenderer);

		// End the frame
		pRenderer->EndFrame();

		// Let some other threads have some time
		Sleep(1);

		g_iStart = g_iEnd;
	}

	LevelManager::Shutdown();

	//delete enemy types loaded
	if( g_paEnemyTypes )
	{
		for( int i = 0; i < g_paEnemyTypes->GetNumElements(); i++)
		{
			EnemyLoadStructure* type = (*g_paEnemyTypes)[i];
			delete type->pModel;
			delete type;
		}

		delete g_paEnemyTypes;
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

GameState* g_pGameState = 0;

//Global game data
kpuFixedArray<EnemyLoadStructure*>* g_paEnemyTypes;

void UpdateGame()
{
	if( !g_pGameState )
	{
		// GameState hasnt been created yet, make the default
		g_pGameState = new GameState_Default();
	}

	g_pGameState->Update(g_fElasped);
}

void DrawGame()
{
	if( g_pGameState )
	{
		g_pGameState->Draw();
	}
}
void LoadEnemyList()
{
	char szFileName[2048];
	TiXmlDocument doc;

	kpuFileManager::GetFullFilePath("/Assets/EnemyData/EnemyMasterList.xml", szFileName, sizeof(szFileName));
	//Load all enemy types and generate them
	if(doc.LoadFile(szFileName))
	{
		for(TiXmlElement* pElement = doc.FirstChildElement(); pElement != 0; pElement = pElement->NextSiblingElement())
		{
			int iCount = atoi(pElement->Attribute("Count"));

			//kpuFixedArray<EnemyLoadStructure>* paEnemyTypes = new kpuFixedArray<EnemyLoadStructure>(iCount);
			g_paEnemyTypes = new kpuFixedArray<EnemyLoadStructure*>(iCount);

			for(TiXmlElement* pChild = pElement->FirstChildElement(); pChild != 0; pChild = pChild->NextSiblingElement())
			{
				const char* szFilename = pChild->FirstChild()->Value();

				LoadEnemyType(szFilename);
			}		

		}
	}
}

void LoadEnemyType(const char* pszFile)
{
	char szFilename[2048];

	kpuFileManager::GetFullFilePath(pszFile, szFilename, sizeof(szFilename) );

	TiXmlDocument doc;

	if( doc.LoadFile(szFilename) )
	{
		TiXmlElement* pElement = doc.FirstChildElement();

		EnemyLoadStructure*	enemyType = new EnemyLoadStructure();

		enemyType->pszName = (char*)pElement->Attribute("Name");
		enemyType->iLevel = atoi(pElement->Attribute("Level"));
		enemyType->iHealth = atoi(pElement->Attribute("Health"));
		enemyType->fSpeed = atof(pElement->Attribute("Speed"));
		enemyType->pModel = new kpgModel();
		enemyType->pModel->Load(pElement->Attribute("Model"));
		enemyType->iDamage = atoi(pElement->Attribute("Damage"));
		enemyType->fAggroRange = atof(pElement->Attribute("Aggro"));
		enemyType->fAttackRange = atof(pElement->Attribute("AtkRange"));
		enemyType->fAttackSpeed = atof(pElement->Attribute("AtkSpeed"));
		enemyType->iDamageType = atof(pElement->Attribute("DamageType"));

		//goto resits
		TiXmlElement* pResits = pElement->FirstChildElement();

		enemyType->iCrushRes = atoi(pResits->Attribute("Crushing"));
		enemyType->iPierceRes = atoi(pResits->Attribute("Piercing"));
		enemyType->iSlashRes = atoi(pResits->Attribute("Slashing"));	
		enemyType->iMentalRes = atoi(pResits->Attribute("Mental"));
		enemyType->iHeatRes = atoi(pResits->Attribute("Heat"));
		enemyType->iColdRes = atoi(pResits->Attribute("Cold"));
		enemyType->iAcidRes = atoi(pResits->Attribute("Acid"));
		enemyType->iViralRes = atoi(pResits->Attribute("Viral"));
		enemyType->iHolyRes = atoi(pResits->Attribute("Holy"));
		enemyType->iWaterRes = atoi(pResits->Attribute("Water"));
		enemyType->iDeathRes = atoi(pResits->Attribute("Death"));
		enemyType->iElectRes = atoi(pResits->Attribute("Electric"));

		g_paEnemyTypes->Add(enemyType);
	}

}
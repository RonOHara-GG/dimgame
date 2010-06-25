// Dimensions.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Dimensions.h"
#include "LevelManager.h"
#include "GameState.h"
#include "GameState_FrontEnd.h"
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

GameState* g_pGameState = 0;
GameState* g_pPendingGameState = 0;

kpiInputManager*		g_pInputManager = 0;
bool					g_bExitGame = false;

//Global game data
extern kpuFixedArray<EnemyLoadStructure*>* g_paEnemyTypes;

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

	// Set the file path
	char szCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurrentDir);
	kpuFileManager::SetRootPath(szCurrentDir);

	// Create the input manager
	g_pInputManager = new kpiInputManager(hWnd);

	// Create the renderer
	kpgRenderer* pRenderer = kpgRenderer::GetInstance();
	pRenderer->Create(hWnd);

	//Load enemies
	LoadEnemyList();

	// Start in the front end game state
	g_pGameState = new GameState_FrontEnd();

	// Setup timing
	LARGE_INTEGER liFrameStart;
	QueryPerformanceFrequency(&liFrameStart);
	double dfCyclesPerMilliSecond = 1.0 / (double)liFrameStart.QuadPart * 1000;
	QueryPerformanceCounter(&liFrameStart);
	float fDeltaTime = 0;

	// Process until exit	
	while( g_pInputManager->Update(fDeltaTime) )
	{
		// Update the window
		UpdateRenderWindow(hWnd);
		if( g_bExitGame )
			break;

		g_pGameState->Update(fDeltaTime);

		// Begin the frame
		pRenderer->BeginFrame();
		
		g_pGameState->Draw();

		// End the frame
		pRenderer->EndFrame();

		// Let some other threads have some time
		Sleep(1);

		// Switch to next game state
		if( g_pPendingGameState )
		{
			delete g_pGameState;
			g_pGameState = g_pPendingGameState;
			g_pPendingGameState = 0;
		}

		LARGE_INTEGER liFrameEnd;
		QueryPerformanceCounter(&liFrameEnd);
		double dfDeltaCycles = (double)(liFrameEnd.QuadPart - liFrameStart.QuadPart);
		fDeltaTime = (float)(dfDeltaCycles * dfCyclesPerMilliSecond);
	}

	delete g_pGameState;

	// Destroy the input manager
	delete g_pInputManager;

	LevelManager::Shutdown();

	//delete enemy types loaded
	if( g_paEnemyTypes )
	{
		for( int i = 0; i < g_paEnemyTypes->GetNumElements(); i++)
		{
			EnemyLoadStructure* type = (*g_paEnemyTypes)[i];
			delete type->pModel;
			delete type->pCollision;
			delete type;
		}

		delete g_paEnemyTypes;
	}

	// Destroy the renderer
	delete pRenderer;	

	// Kill the window
	CloseRenderWindow(hWnd);

	return 0;
}

//Global game data
kpuFixedArray<EnemyLoadStructure*>* g_paEnemyTypes;

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
		enemyType->fSpeed = (float)atof(pElement->Attribute("Speed"));
		enemyType->pModel = new kpgModel();
		enemyType->pModel->Load(pElement->Attribute("Model"));
		enemyType->pCollision = new kpgModel();
		enemyType->pCollision->Load(pElement->Attribute("Collision"));
		enemyType->iDamage = (int)atoi(pElement->Attribute("Damage"));
		enemyType->fAggroRange = (float)atof(pElement->Attribute("Aggro"));
		enemyType->fAttackRange = (float)atof(pElement->Attribute("AtkRange"));
		enemyType->fAttackSpeed = (float)atof(pElement->Attribute("AtkSpeed"));
		enemyType->iDamageType = atoi(pElement->Attribute("DamageType"));

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

void InputEvent(eInputEventType type, u32 button)
{
	if( g_pGameState )
		g_pGameState->HandleInputEvent(type, button);
}

void ChangeGameState(GameState* newGameState)
{
	if( g_pPendingGameState )
		delete g_pPendingGameState;
	g_pPendingGameState = newGameState;
}
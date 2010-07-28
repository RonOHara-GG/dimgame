// Dimensions.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Dimensions.h"
#include "LevelManager.h"
#include "GameState.h"
#include "GameState_FrontEnd.h"
#include "GameState_GamePlay.h"
#include "LoadStructures.h"
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
#include "Common/Utility/kpuXmlParser.h"

#include <math.h>
#include <d3dx9math.h>
#include <time.h>

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
	//Seed random
	srand((unsigned int)time(NULL));

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
	double dfCyclesPerMilliSecond = 1.0 / (double)liFrameStart.QuadPart;
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
		liFrameStart = liFrameEnd;
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
	//Load all enemy types and generate them
	kpuXmlParser* pParser = new kpuXmlParser();
	if( pParser->LoadFile("/Assets/EnemyData/EnemyMasterList.xml") )
	{
		while( pParser->HasElement() )
		{		
			int iCount = pParser->GetAttributeAsInt("Count");

			g_paEnemyTypes = new kpuFixedArray<EnemyLoadStructure*>(iCount);

			pParser->FirstChildElement();

			while( pParser->HasElement() )
			{
				LoadEnemyType(pParser->GetChildValue());
				pParser->NextSiblingElement();
			}	

			pParser->Parent();
			pParser->NextSiblingElement();
		}

	}

	delete pParser;
}

void LoadEnemyType(const char* pszFile)
{	
	kpuXmlParser* pParser = new kpuXmlParser();
	if( pParser->LoadFile(pszFile) )
	{

		EnemyLoadStructure*	enemyType = new EnemyLoadStructure();

		enemyType->pszName = (char*)pParser->GetAttribute("Name");
		enemyType->iLevel = pParser->GetAttributeAsInt("Level");
		enemyType->iHealth = pParser->GetAttributeAsInt("Health");
		enemyType->fSpeed = pParser->GetAttributeAsFloat("Speed");
		enemyType->pModel = new kpgModel();
		enemyType->pModel->Load(pParser->GetAttribute("Model"));
		enemyType->pCollision = new kpgModel();
		enemyType->pCollision->Load(pParser->GetAttribute("Collision"));
		enemyType->iDamage = pParser->GetAttributeAsInt("Damage");
		enemyType->fAggroRange = pParser->GetAttributeAsFloat("Aggro");
		enemyType->fAttackRange = pParser->GetAttributeAsFloat("AtkRange");
		enemyType->fAttackSpeed = pParser->GetAttributeAsFloat("AtkSpeed");
		enemyType->iDamageType = pParser->GetAttributeAsInt("DamageType");

		//goto resits
		pParser->FirstChildElement();

		enemyType->iCrushRes = pParser->GetAttributeAsInt("Crushing");
		enemyType->iPierceRes = pParser->GetAttributeAsInt("Piercing");
		enemyType->iSlashRes = pParser->GetAttributeAsInt("Slashing");	
		enemyType->iMentalRes = pParser->GetAttributeAsInt("Mental");
		enemyType->iHeatRes = pParser->GetAttributeAsInt("Heat");
		enemyType->iColdRes = pParser->GetAttributeAsInt("Cold");
		enemyType->iAcidRes = pParser->GetAttributeAsInt("Acid");
		enemyType->iViralRes = pParser->GetAttributeAsInt("Viral");
		enemyType->iHolyRes = pParser->GetAttributeAsInt("Holy");
		enemyType->iWaterRes = pParser->GetAttributeAsInt("Water");
		enemyType->iDeathRes = pParser->GetAttributeAsInt("Death");
		enemyType->iElectRes = pParser->GetAttributeAsInt("Electric");

		g_paEnemyTypes->Add(enemyType);
	}

	delete pParser;
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
void Terminate()
{
	g_bExitGame = true; 
}


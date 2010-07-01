#include "StdAfx.h"
#include "GameState_FrontEnd.h"
#include "Common\Input\kpiInputManager.h"
#include "Common\graphics\kpgUIManager.h"
#include "Common\graphics\kpgUIWindow.h"
#include "Common\graphics\kpgRenderer.h"
#include "GameState_SpaceStation.h"
#include "GameState_GamePlay.h"
#include "LoadStructures.h"
#include "PlayerCharacter.h"
#include "Common/Utility/kpuFileManager.h"
#include "Common/Graphics/kpgModel.h"
#include "LevelManager.h"
#include "Level.h"

static const u32 s_uHash_SpaceStation = 0x6507d2d3;
static const u32 s_uHash_Moon = 0x7c892f9e;
static const u32 s_uHash_Earth = 0xd16bff9;

GameState_FrontEnd::GameState_FrontEnd(void)
{
	LevelManager* pLevelManager = LevelManager::GetInstance();
	m_pCurrentLevel = pLevelManager->LoadLevel(eLevelID::eLID_FrontEnd);

	//Load background
	LoadBackground("Assets/UI/FrontEnd/FrontEndBackground.xml");
	m_pUIManager = new kpgUIManager();
	m_pUIManager->LoadWindows("Assets/UI/FrontEnd/FrontEndUI.xml");
	m_plCurrentModel = 0;

	LoadAllPlayerModels("Assets//Player//PlayerModels.xml");
	m_bCharacterCreation = false;

	kpgRenderer* pRenderer = kpgRenderer::GetInstance();

	pRenderer->GetDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
    pRenderer->GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );

	// for now, setup a temp light
	kpgLight* pDummyLight = new kpgLight(kpgLight::eLT_Directional);
	kpuVector vLightDir(0, -1, 0, 0);
	vLightDir.Normalize();
	pDummyLight->SetDirection(vLightDir);
	pDummyLight->SetColor(kpuVector(0.0f, 0.0f, 0.0f, 0.75f));
	pRenderer->SetLight(0, pDummyLight);

	//define character view matrix
	m_mCharacterView.LookAt(kpuVector(0.0f, 0.9f, -4.0f, 0.0f), kpuVector(0.0f, 0.9f, 0.0f, 0.0f), kpuv_OneY);
}

GameState_FrontEnd::~GameState_FrontEnd(void)
{
	delete m_pUIManager;

	kpuLinkedList* plNext = m_lPlayerModels.Next();
	while( plNext )
	{
		if( plNext->GetPointer() != m_plCurrentModel )
		{
			delete plNext->GetPointer();
			plNext->SetPointer(0);
		}
		plNext = plNext->Next();
	}

	plNext = m_lBgObjects.Next();
	while( plNext )
	{		
		Planet* pPlanet = (Planet*)plNext->GetPointer();
		
		//delete model
		delete pPlanet->m_pModel;

		//delete planet
		delete pPlanet;

		plNext->SetPointer(0);
		
		plNext = plNext->Next();
	}
}

void GameState_FrontEnd::MouseUpdate(int X, int Y)
{
}

void GameState_FrontEnd::Update(float fDeltaTime)
{
	//rotate the background
	kpuLinkedList* pNext = m_lBgObjects.Next();

	while( pNext )
	{
		Planet* pPlanet = (Planet*)pNext->GetPointer();

		pPlanet->m_fOrbit += pPlanet->m_fOrbitSpeed * fDeltaTime;

		if( pPlanet->m_fOrbit >= PI * 2 )
			pPlanet->m_fOrbit -= PI * 2;

		//get the new position
		kpuVector vPos(cosf(pPlanet->m_fOrbit), 0.0f, sinf(pPlanet->m_fOrbit), 0.0f);
		vPos *= pPlanet->m_fDistance;

		if( pPlanet->m_pParent )
			vPos += pPlanet->m_pParent->m_pModel->GetPosition();

		pPlanet->m_pModel->SetPosition(vPos);
		float fDist = vPos.Length();

		pPlanet->m_fRotation += pPlanet->m_fRotationSpeed * fDeltaTime;

		if( pPlanet->m_fRotation >= PI * 2 )
			pPlanet->m_fRotation -= PI * 2;

		pPlanet->m_pModel->Rotate(0.0f, pPlanet->m_fRotation, 0.0f);

		pNext = pNext->Next();
	}

	kpuVector vPos = m_pLookAt->m_pModel->GetPosition();
	kpuVector vDir = vPos - m_pLookAt->m_pParent->m_pModel->GetPosition();;
	vDir.Normalize();

	m_mBgView.LookAt(vPos + vDir * 10.0f, vPos, kpuv_OneY);	
}

void GameState_FrontEnd::LoadBackground(const char* szFile)
{
	TiXmlDocument doc;
	char szFileName[2048];
	kpuFileManager::GetFullFilePath(szFile, szFileName, sizeof(szFileName));

	if( doc.LoadFile(szFileName) )
	{
		Planet* pEarth;

		TiXmlElement* pElement = doc.FirstChildElement();
	
		for(pElement = pElement->FirstChildElement(); pElement != 0; pElement = pElement->NextSiblingElement() )
		{
			Planet* pPlanet = new Planet();
			pPlanet->m_pModel = new kpgModel();
			pPlanet->m_pModel->Load(pElement->Attribute("File"));
			pPlanet->m_fDistance = (float)atof(pElement->Attribute("Distance"));

			//calculate oribit speed	
			float fOrbitPeriod = (float)atof(pElement->Attribute("Orbit"));		
			pPlanet->m_fOrbitSpeed = 0.0f;

			if( fOrbitPeriod > 0.0f )
				pPlanet->m_fOrbitSpeed = PI * 2 / fOrbitPeriod;

			//calaculate rotation speed
			pPlanet->m_fRotationSpeed = (float)atof(pElement->Attribute("Rotation"));

			if( pPlanet->m_fRotationSpeed > 0.0f )
				pPlanet->m_fRotationSpeed= PI * 2 / pPlanet->m_fRotationSpeed;
			else
				pPlanet->m_fRotationSpeed = 0.0f;

			pPlanet->m_pParent = 0;
			pPlanet->m_fOrbit = 0.0f;
			pPlanet->m_fRotation = 0.0f;

			const char* szName = pElement->Attribute("Name");
			u32 uHash = StringHash(szName);

			if( uHash == s_uHash_Earth )
				pEarth = pPlanet;
			else if( uHash == s_uHash_Moon  )
				pPlanet->m_pParent = pEarth;
			else if( uHash == s_uHash_SpaceStation )
			{
				pPlanet->m_pParent = pEarth;
				m_pLookAt = pPlanet;
			}

			kpuVector vParentOffset(0.0f, 0.0f, 0.0f, 0.0f);

			if( pPlanet->m_pParent )
				vParentOffset = pPlanet->m_pParent->m_pModel->GetPosition();

			pPlanet->m_pModel->SetPosition(kpuv_OneX * pPlanet->m_fDistance + vParentOffset);
			m_lBgObjects.AddTail(pPlanet);		
			
		}
		
		kpgRenderer* pRenderer = kpgRenderer::GetInstance();
		m_mBgProjection.Perspective(45.0f, pRenderer->GetScreenWidth() / pRenderer->GetScreenHeight(), 0.001f, 100000000.0f);
	}

}

void GameState_FrontEnd::Draw()
{
	kpgRenderer* pRenderer = kpgRenderer::GetInstance();
	pRenderer->SetAmbientLightColor(kpuVector(0.75f, 0.75f, 0.75f, 1.0f));	

	kpuMatrix mWorld;
	mWorld.Identity();

	pRenderer->SetProjectionMatrix(m_mBgProjection);
	pRenderer->SetViewMatrix(m_mBgView);
	pRenderer->SetWorldMatrix(mWorld);

	//draw level
	if( m_pCurrentLevel )
		m_pCurrentLevel->Draw(pRenderer);

	//Draw background
	kpuLinkedList* pNext = m_lBgObjects.Next();

	while( pNext )
	{
		((Planet*)pNext->GetPointer())->m_pModel->Draw();
		pNext = pNext->Next();
	}

	if( m_bCharacterCreation )
	{
		kpuMatrix mProjection;
		mProjection.Perspective(45.0f, pRenderer->GetScreenWidth() / pRenderer->GetScreenHeight(), 0.001f, 10000.0f);
		pRenderer->SetProjectionMatrix(m_mBgProjection);
		pRenderer->SetViewMatrix(m_mCharacterView);
		pRenderer->SetWorldMatrix(mWorld);
		((kpgModel*)m_plCurrentModel->GetPointer())->Draw();
	}

	m_pUIManager->Draw(pRenderer);
}

void GameState_FrontEnd::AddActor(Actor* pActor)
{
}

bool GameState_FrontEnd::LoadMostRecentSave()
{
	WIN32_FIND_DATA data;	
	HANDLE hResult = FindFirstFile("SavedGames", &data);

	if( hResult != INVALID_HANDLE_VALUE )
	{
		WIN32_FIND_DATA nextData;		

		while( FindNextFile(hResult, &nextData) )
		{
			//TODO: add file extension check for dimension's saved game file type
			if( CompareFileTime(&data.ftLastWriteTime, &nextData.ftLastWriteTime) == -1 )
				data = nextData;
		}

		return LoadGame(data.cFileName);
	}

	return false;
}

bool GameState_FrontEnd::LoadGame(const char* szFile)
{
	FILE* pFile;
	fopen_s(&pFile, szFile, "rb");

	if( pFile )
	{
		PlayerLoadStructure playerData;
		int iResult = fread(&playerData, sizeof(PlayerLoadStructure), 1, pFile);

		fclose(pFile);
		if( iResult == sizeof(PlayerLoadStructure) )
		{
			m_pPlayer = new PlayerCharacter(playerData);
			ChangeGameState(new GameState_SpaceStation(m_pPlayer));
			return true;
		}
	}
	return false;
}

void GameState_FrontEnd::NextCharacterModel()
{
	if( m_plCurrentModel )
	{
		if( m_plCurrentModel->Next() && m_plCurrentModel->Next()->GetPointer() )
		{
			m_plCurrentModel = m_plCurrentModel->Next();
			return;
		}
	}

	//Get the first model in the list
	m_plCurrentModel = m_lPlayerModels.Next();
}

void GameState_FrontEnd::PreviousCharacterModel()
{
	if( m_plCurrentModel )
	{
		if( m_plCurrentModel->Prev() && m_plCurrentModel->Prev()->GetPointer() )
		{
			m_plCurrentModel = m_plCurrentModel->Prev();
			return;
		}
	}

	//Get the last model in the list
	m_plCurrentModel = m_lPlayerModels.Last();
}

void GameState_FrontEnd::LoadAllPlayerModels(const char* szFile)
{
	TiXmlDocument doc;
	char szFileName[2048];
	kpuFileManager::GetFullFilePath(szFile, szFileName, sizeof(szFileName));

	if( doc.LoadFile(szFileName) )
	{
		for(TiXmlElement* pElement = doc.FirstChildElement()->FirstChildElement(); pElement != 0; pElement = pElement->NextSiblingElement() )
		{
			kpgModel* pModel = new kpgModel();
			pModel->Load(pElement->Attribute("File"));
			m_lPlayerModels.AddTail(pModel);
		}

		m_plCurrentModel = m_lPlayerModels.Next();
	}
}

bool GameState_FrontEnd::HandleInputEvent(eInputEventType type, u32 button)
{
	u32 uResult = m_pUIManager->HandleInputEvent(type, button);

	if( uResult == 0 )
		return true;

	//try and handle result
	switch( uResult )
	{
	case CE_ENTER_GAME:
		{
			m_pPlayer = new PlayerCharacter((kpgModel*)m_plCurrentModel->GetPointer(), m_szName, m_eStartClass);
			ChangeGameState(new GameState_SpaceStation(m_pPlayer));
			return true;
		}
	case CE_ENTER_CHARACTER_CREATION:
		{
			m_bCharacterCreation = true;
			m_pUIManager->NewWindow(CE_ENTER_CHARACTER_CREATION);
			return true;
		}
	case CE_LOAD_MOST_RECENT:
		{
			LoadMostRecentSave();
			return true;
		}
	case CE_NEXT_PLAYER_MODEL:
		{
			NextCharacterModel();
			return true;
		}
		case CE_PREVIOUS_PLAYER_MODEL:
		{
			PreviousCharacterModel();
			return true;
		}
	case CE_EXIT:
		{
			Terminate();
			return true;
		}
	}

	
	return false;
}
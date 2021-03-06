#include "StdAfx.h"
#include "GameState_FrontEnd.h"
#include "GameState_SpaceStation.h"
#include "GameState_Main.h"
#include "LoadStructures.h"
#include "PlayerCharacter.h"
#include "LevelManager.h"
#include "Level.h"
#include "Common\Input\kpiInputManager.h"
#include "Common\graphics\kpgUIManager.h"
#include "Common\graphics\kpgUIWindow.h"
#include "Common\graphics\kpgRenderer.h"
#include "Common\Utility\kpuCameraController.h"
#include "Common\Utility\kpuFileManager.h"
#include "Common\Utility\kpuXmlParser.h"
#include "Common\Graphics\kpgModel.h"
#include "Common\Graphics\kpgAnimationManager.h"
#include "PlayerClass.h"

static const u32 s_uHash_Planet =	0xca551fa9;
static const u32 s_uHash_true =		0x7c9e9fe5;

GameState_FrontEnd::GameState_FrontEnd(void)
{
	LevelManager* pLevelManager = LevelManager::GetInstance();
	m_pCurrentLevel = pLevelManager->LoadLevel(eLID_FrontEnd);

	m_pUIManager = new kpgUIManager();
	m_pUIManager->LoadWindows("Assets/UI/FrontEnd/FrontEndUI.xml");
	m_plCurrentModel = 0;

	LoadAllPlayerModels("Assets//Player//PlayerModels.xml");
	m_bCharacterCreation = false;

	kpgRenderer* pRenderer = kpgRenderer::GetInstance();
	m_mProjection.Perspective(35.0f, pRenderer->GetScreenWidth() / pRenderer->GetScreenHeight(), 1.0f, 100000000.0f);
	m_pCamera = new kpuCameraController();
	pRenderer->SetCullMode(kpgRenderer::eCM_None);

	SetupOrbitalData("Assets/FrontEnd/OrbitalData.xml");

	//Character model view
	// setup render state
	pRenderer->SetCullMode(kpgRenderer::eCM_None);
    pRenderer->GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_mCharacterView.LookAt(kpuVector(0.0f, 0.9f, 4.0f, 0.0f), kpuVector(0.0f, 0.9f, 0.0f, 0.0f), kpuv_OneY);

	m_uClassFlags = 0;

}

GameState_FrontEnd::~GameState_FrontEnd(void)
{
	delete m_pUIManager;
	//delete m_pCurrentLevel;

	kpuLinkedList* plNext = m_lPlayerModels.Next();
	while( plNext )
	{
		if( plNext->GetPointer() != m_plCurrentModel->GetPointer() )
		{
			delete plNext->GetPointer();
			plNext->SetPointer(0);
		}
		plNext = plNext->Next();
	}
}

void GameState_FrontEnd::MouseUpdate(int X, int Y)
{
}

void GameState_FrontEnd::Update(float fDeltaTime)
{
	// Update my planets rotations & positions
	for( int i = 1; i < NUM_PLANETS; i++ )			// Skip the sun
	{
		// Update rotation
		if( m_aPlanets[i].m_fRotationalPeriod > 0 )
		{
			float fRotationPercent = fDeltaTime / m_aPlanets[i].m_fRotationalPeriod;
			float fRotation = fRotationPercent * 2 * PI;

			kpuMatrix mRot;
			mRot.SetRotationY(fRotation);
			
			kpuMatrix mModel = m_aPlanets[i].m_pModel->GetMatrix();
			mModel.RotateLocal(mRot);
			m_aPlanets[i].m_pModel->SetMatrix(mModel);
		}

		// Update orbit
		if( m_aPlanets[i].m_pOrbit && m_aPlanets[i].m_fOrbitalPeriod > 0 )
		{
			Orbit(i, fDeltaTime);
		}
	}

	// Update camera position
	kpuVector vSpaceStation = m_aPlanets[PLANET_SPACE_STATION].m_pModel->GetPosition();
	kpuVector vSpaceStationToEarth = m_aPlanets[PLANET_EARTH].m_pModel->GetPosition() - vSpaceStation;
	vSpaceStationToEarth.Normalize();

	kpuVector vViewDir = kpuVector::Cross(vSpaceStationToEarth, kpuv_OneY);
	
	vViewDir.RotateY(PI * 0.0625f);

	kpuVector vUp = kpuVector::Cross(vViewDir, vSpaceStationToEarth);

	vViewDir *= -8;
	vViewDir += vSpaceStation;

	m_pCamera->SetLookAt(vSpaceStation);
	m_pCamera->SetLocation(vViewDir);
	m_pCamera->SetUp(vUp);
	m_pCamera->Update();

	((kpgModel*)m_plCurrentModel->GetPointer())->PlayAnimation(StringHash("Animation"));
	((kpgModel*)m_plCurrentModel->GetPointer())->UpdateAnimations(fDeltaTime);
}

void GameState_FrontEnd::SetupOrbitalData(const char* szFile)
{
	kpuXmlParser* pParser = new kpuXmlParser();

	bool bLoaded = pParser->LoadFile(szFile);
	assert(bLoaded);
	
	int iPlanet = 0;
	while(pParser->HasElement() )
	{	
		u32 iTag = pParser->GetValueAsInt();
		if( iTag == s_uHash_Planet )
		{
			// Find this planet in the level			
			u32 iName = pParser->GetAttributeAsInt("Name");
			m_aPlanets[iPlanet].m_pModel = m_pCurrentLevel->FindModelByName(iName);

			m_aPlanets[iPlanet].m_fRotationalPeriod = pParser->GetAttributeAsFloat("RotationPeriod");
			m_aPlanets[iPlanet].m_fOrbitalPeriod = pParser->GetAttributeAsFloat("OrbitalPeriod");			
			m_aPlanets[iPlanet].m_bOrbitReverse = pParser->GetAttributeAsInt("OrbitReverse") == s_uHash_true;
			
			// Find the orbit body too
			iName = pParser->GetAttributeAsInt("Orbits");
			m_aPlanets[iPlanet++].m_pOrbit = m_pCurrentLevel->FindModelByName(iName);
		}

		pParser->NextSiblingElement();
	}



	// Setup orbital axes
	for( int i = 1; i < NUM_PLANETS; i++ )		// Skip the sun at index 0
	{
		if( m_aPlanets[i].m_pOrbit )
		{			
			kpuVector vBetween = m_aPlanets[i].m_pModel->GetPosition() - m_aPlanets[i].m_pOrbit->GetPosition();
			kpuVector vNormalBetween = kpuVector::Normalize(vBetween);
			kpuVector vFront = kpuVector::Cross(vNormalBetween, kpuv_OneY);	
			vFront.Normalize();
			m_aPlanets[i].m_vOrbitAxis = kpuVector::Cross(vFront, vNormalBetween);
			m_aPlanets[i].m_vOrbitAxis.Normalize();
		}
	}

	delete pParser;
}

void GameState_FrontEnd::Orbit(int iPlanet, float fDeltaTime)
{
	float fOrbitPercent = fDeltaTime / m_aPlanets[iPlanet].m_fOrbitalPeriod ;
	float fOrbitRotation = fOrbitPercent * 2 * PI;

	if( m_aPlanets[iPlanet].m_bOrbitReverse )
		fOrbitRotation *= -1;

	kpuVector vBetween = m_aPlanets[iPlanet].m_pModel->GetPosition() - m_aPlanets[iPlanet].m_pOrbit->GetPosition();

	kpuMatrix mRot;
	mRot.SetRotationAboutAxis(m_aPlanets[iPlanet].m_vOrbitAxis, fOrbitRotation);
	vBetween *= mRot;

	vBetween += m_aPlanets[iPlanet].m_pOrbit->GetPosition();
	m_aPlanets[iPlanet].m_pModel->SetPosition(vBetween);

	kpuMatrix mLocalRot;
	mLocalRot.SetRotationY(fOrbitRotation);
	
	kpuMatrix mModel = m_aPlanets[iPlanet].m_pModel->GetMatrix();
	mModel.RotateLocal(mLocalRot);
	m_aPlanets[iPlanet].m_pModel->SetMatrix(mModel);

	// update all things orbiting this planet too
	for( int i = 0; i < NUM_PLANETS; i++ )
	{
		if( m_aPlanets[i].m_pOrbit == m_aPlanets[iPlanet].m_pModel )
		{
			vBetween = m_aPlanets[i].m_pModel->GetPosition() - m_aPlanets[iPlanet].m_pOrbit->GetPosition();
			vBetween *= mRot;
			vBetween += m_aPlanets[iPlanet].m_pOrbit->GetPosition();
			m_aPlanets[i].m_pModel->SetPosition(vBetween);

			m_aPlanets[i].m_vOrbitAxis *= mRot;
			
			kpuMatrix mModel = m_aPlanets[i].m_pModel->GetMatrix();
			mModel.RotateLocal(mLocalRot);
			m_aPlanets[i].m_pModel->SetMatrix(mModel);
		}
	}
}

void GameState_FrontEnd::Draw()
{
	kpgRenderer* pRenderer = kpgRenderer::GetInstance();
	pRenderer->SetAmbientLightColor(kpuVector(0.75f, 0.75f, 0.75f, 1.0f));	

	pRenderer->SetProjectionMatrix(m_mProjection);
	pRenderer->SetViewMatrix(m_pCamera->GetViewMatrix());

	m_pCurrentLevel->Draw(pRenderer);

	if( m_bCharacterCreation )
	{
		pRenderer->SetViewMatrix(m_mCharacterView);
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
	kpuXmlParser* pParser = new kpuXmlParser();

	if( pParser->LoadFile(szFile) )
	{
		pParser->FirstChildElement();
		while( pParser->HasElement() )
		{		
			kpgModel* pModel = new kpgModel();
			pModel->Load(pParser->GetAttribute("File"));
			m_lPlayerModels.AddTail(pModel);	

			//get the shader and animations
			pParser->FirstChildElement();

			while(pParser->HasElement() )
			{
				if( (u32)pParser->GetValueAsInt() == StringHash("Shader") )
					pModel->SetShader(pParser->GetChildValue());
				else if( (u32)pParser->GetValueAsInt() == StringHash("Animation") )
					kpgAnimationManager::GetInstance()->LoadAnimation( pParser->GetChildValue(), pParser->GetValueAsInt());

				pParser->NextSiblingElement();
			}

			pParser->Parent();
		
			pParser->NextSiblingElement();
		}
		pParser->Parent();

		m_plCurrentModel = m_lPlayerModels.Next();
	}

	delete pParser;
}

bool GameState_FrontEnd::HandleInputEvent(eInputEventType type, u32 button)
{
	EventParam result = m_pUIManager->HandleInputEvent(type, button);

	if( result.m_uEvent == 0 )
		return true;

	//try and handle result
	switch( result.m_uEvent )
	{
	case CE_ENTER_GAME:
		{
			m_pPlayer = new PlayerCharacter((kpgModel*)m_plCurrentModel->GetPointer(), m_uClassFlags);
			ChangeGameState(new GameState_SpaceStation(m_pPlayer));
			return true;
		}
	case CE_ENTER_CHARACTER_CREATION:
		{
			m_bCharacterCreation = true;
			m_pUIManager->CloseAll();
			m_pUIManager->OpenUIWindow(CE_ENTER_CHARACTER_CREATION);
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
	case CE_SELECT_CLASS:
		{
			u32 uFlag = 1 << result.m_uParam;
			if( ( m_uClassFlags & uFlag ) != 0 )
				m_uClassFlags &= ~uFlag;
			else
				m_uClassFlags |= uFlag;

			break;
		}
	case CE_EXIT:
		{
			Terminate();
			return true;
		}
	}

	
	return false;
}
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

GameState_FrontEnd::GameState_FrontEnd(void)
{
	m_pUIManager = new kpgUIManager();
	m_pUIManager->LoadWindows("Assets/UI/FrontEnd/FrontEndUI.xml");
	m_plCurrentModel = 0;

	LoadAllPlayerModels("Assets//Player//PlayerModels.xml");
	m_bCharacterCreation = false;
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
}

void GameState_FrontEnd::MouseUpdate(int X, int Y)
{
}

void GameState_FrontEnd::Update(float fDeltaTime)
{
}

void GameState_FrontEnd::Draw()
{
	m_pUIManager->Draw(kpgRenderer::GetInstance());

	if( m_bCharacterCreation )
		((kpgModel*)m_plCurrentModel->GetPointer())->Draw();
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
		if( m_plCurrentModel->Next()->GetPointer() )
		{
			m_plCurrentModel = m_plCurrentModel->Next();
			return;
		}
	}

	//Get the first model in the list
	m_plCurrentModel = m_lPlayerModels.Next();
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
	case CE_EXIT:
		{
			Terminate();
			return true;
		}
	}

	
	return false;
}
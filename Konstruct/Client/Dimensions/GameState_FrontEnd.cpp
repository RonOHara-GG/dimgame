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

GameState_FrontEnd::GameState_FrontEnd(void)
{
	m_pUIManager = new kpgUIManager();
	m_pUIManager->LoadWindows("Assets/UI/FrontEnd/FrontEndUI.xml");
}

GameState_FrontEnd::~GameState_FrontEnd(void)
{
	delete m_pUIManager;
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
		FILETIME currentFileTime;	
		SYSTEMTIME sysTime;
		GetSystemTime(&sysTime);
		SystemTimeToFileTime(&sysTime, &currentFileTime);

		//Store times in a sturcture that math can be used on
		ULARGE_INTEGER currentTime;
		ULARGE_INTEGER bestTime;
		ULARGE_INTEGER nextTime;
		currentTime.HighPart = currentFileTime.dwHighDateTime;
		currentTime.LowPart = currentFileTime.dwLowDateTime;
		bestTime.HighPart = data.ftLastWriteTime.dwHighDateTime;
		bestTime.LowPart = data.ftLastWriteTime.dwLowDateTime;


		while( FindNextFile(hResult, &nextData) )
		{
			
			nextTime.HighPart = nextData.ftLastWriteTime.dwHighDateTime;
			nextTime.LowPart = nextData.ftLastWriteTime.dwLowDateTime;

			//TODO: add file extension check for dimension's saved game file type
			if( currentTime.QuadPart - nextTime.QuadPart < currentTime.QuadPart - bestTime.QuadPart )
			{
				data = nextData;
				bestTime.HighPart = data.ftLastWriteTime.dwHighDateTime;
				bestTime.LowPart = data.ftLastWriteTime.dwLowDateTime;
			}


		}

		return LoadGame(data.cAlternateFileName);
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
			return true;
		}
	}
	return false;
}

bool GameState_FrontEnd::HandleInputEvent(eInputEventType type, u32 button)
{
	u32 uResult = m_pUIManager->HandleInputEvent(type, button);

	if( uResult == 0 )
		return true;

	//try and handle result
	switch( uResult )
	{
	case CE_NEW_GAME:
		{
			m_pPlayer = new PlayerCharacter((kpgModel*)m_plCurrentModel->GetPointer(), m_szName, m_eStartClass);
			ChangeGameState(new GameState_SpaceStation(m_pPlayer));
			return true;
		}
	case CE_LOAD_MOST_RECENT:
		{
			LoadMostRecentSave();
			return true;
		}
	case CE_EXIT:
		{
			return true;
		}
	}

	
	return false;
}
#include "StdAfx.h"
#include "GameState_Main.h"
#include "LevelManager.h"
#include "Level.h"
#include "PlayerCharacter.h"
#include "Common\Utility\kpuQuadTree.h"
#include "Common\Graphics\kpgUIManager.h"

GameState_Main::GameState_Main(PlayerCharacter* pPlayer, eLevelID eLID)
{
	LevelManager* pLevelManager = LevelManager::GetInstance();
	m_pCurrentLevel = pLevelManager->LoadLevel(eLID);
	m_pPlayer = pPlayer;
	m_pCurrentLevel->GetQuadTree()->Add(m_pPlayer);
	m_paActors = new kpuArrayList<Actor*>();

	m_paActors->Add(m_pPlayer);
	m_pCurrentLevel->GenerateEnemies(m_paActors);

	//update UI's inventory data source from player
	m_pUIManager->SetDataSource("InventoryIcons", m_pPlayer->GetInventoryIcons());
}

GameState_Main::~GameState_Main(void)
{		
}
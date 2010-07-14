#include "StdAfx.h"
#include "GameState_SpaceStation.h"
#include "LevelManager.h"
#include "Level.h"
#include "PlayerCharacter.h"
#include "Common\Utility\kpuQuadTree.h"
#include "Common\Graphics\kpgUIManager.h"

GameState_SpaceStation::GameState_SpaceStation(PlayerCharacter* pPlayer)
{
	LevelManager* pLevelManager = LevelManager::GetInstance();
	m_pCurrentLevel = pLevelManager->LoadLevel(eLID_SpaceStation);
	m_pPlayer = pPlayer;
	m_pCurrentLevel->GetQuadTree()->Add(m_pPlayer);
	m_paActors = new kpuArrayList<Actor*>();

	m_paActors->Add(m_pPlayer);
	//m_pCurrentLevel->GenerateEnemies(m_paActors); 

	//update UI's inventory data source from player
	m_pUIManager->SetDataSource("InventoryIcons", m_pPlayer->GetInventoryIcons());
}

GameState_SpaceStation::~GameState_SpaceStation(void)
{		
}
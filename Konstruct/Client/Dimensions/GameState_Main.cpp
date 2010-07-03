#include "StdAfx.h"
#include "GameState_Main.h"
#include "LevelManager.h"
#include "Level.h"
#include "PlayerCharacter.h"
#include "Common\Utility\kpuQuadTree.h"

GameState_Main::GameState_Main(PlayerCharacter* pPlayer, eLevelID eLID)
{
	LevelManager* pLevelManager = LevelManager::GetInstance();
	m_pCurrentLevel = pLevelManager->LoadLevel(eLID);
	m_pPlayer = new PlayerCharacter();
	m_pCurrentLevel->GetQuadTree()->Add(m_pPlayer);
	m_paActors = new kpuArrayList<Actor*>();

	m_paActors->Add(m_pPlayer);
	m_pCurrentLevel->GenerateEnemies(m_paActors); 
}

GameState_Main::~GameState_Main(void)
{		
}
#include "StdAfx.h"
#include "GameState_SpaceStation.h"
#include "LevelManager.h"
#include "Level.h"
#include "PlayerCharacter.h"
#include "Common\Utility\kpuQuadTree.h"

GameState_SpaceStation::GameState_SpaceStation(PlayerCharacter* pPlayer)
{
	LevelManager* pLevelManager = LevelManager::GetInstance();
	m_pCurrentLevel = pLevelManager->LoadLevel(eLID_SpaceStation);
	m_pPlayer = pPlayer;
	m_pCurrentLevel->GetQuadTree()->Add(m_pPlayer);
	m_paActors = new kpuArrayList<Actor*>();

	m_paActors->Add(m_pPlayer);
	//m_pCurrentLevel->GenerateEnemies(m_paActors); 
}

GameState_SpaceStation::~GameState_SpaceStation(void)
{		
}
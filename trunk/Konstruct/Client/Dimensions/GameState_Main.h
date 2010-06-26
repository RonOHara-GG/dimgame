#pragma once
#include "GameState_GamePlay.h"
#include "LevelManager.h"

class GameState_Main : public GameState_GamePlay
{
public:
	GameState_Main(PlayerCharacter* pPlayer, eLevelID eLID);
	virtual ~GameState_Main(void);

protected:
};

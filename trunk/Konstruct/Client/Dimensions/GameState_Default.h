#pragma once
#include "GameState.h"

class Level;
class PlayerCharacter;

class GameState_Default : public GameState
{
public:
	GameState_Default(void);
	virtual ~GameState_Default(void);

	virtual void MouseUpdate(int X, int Y);

	virtual void Update();
	virtual void Draw();

protected:
	Level*				m_pCurrentLevel;

	PlayerCharacter*	m_pPlayer;
};

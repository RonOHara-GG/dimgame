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

	virtual void Update(float fGameTime);
	virtual void Draw();

	virtual Level* GetLevel()		{ return m_pCurrentLevel; }

protected:
	Level*				m_pCurrentLevel;

	PlayerCharacter*	m_pPlayer;
};

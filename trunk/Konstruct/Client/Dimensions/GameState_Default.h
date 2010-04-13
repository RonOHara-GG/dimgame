#pragma once
#include "GameState.h"
#include "Common/Utility/kpuArrayList.h"

class Level;
class Enemy;
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

	virtual void AddEnemy(Enemy* pEnemy);

protected:
	Level*					m_pCurrentLevel;

	PlayerCharacter*		m_pPlayer;
	kpuArrayList<Enemy*>*	m_paEnemies;
};

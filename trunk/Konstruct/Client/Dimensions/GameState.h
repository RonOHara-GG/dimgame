#pragma once

class Level;
class Enemy;
class PlayerCharacter;

class GameState
{
public:
	GameState(void)					{};
	virtual ~GameState(void)		{};

	virtual void MouseUpdate(int X, int Y) = 0;

	virtual void Update(float fGameTime) = 0;
	virtual void Draw() = 0;

	virtual Level* GetLevel() = 0;

	virtual void AddEnemy(Enemy* pEnemy) = 0;
	virtual PlayerCharacter* GetPlayer() = 0;
	//virtual kpuArrayList<Enemy*>*	GetEnemies() = 0;
};

extern GameState* g_pGameState;

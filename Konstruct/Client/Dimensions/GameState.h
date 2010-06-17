#pragma once

class Level;
class Actor;
class PlayerCharacter;
enum eInputEventType;

class GameState
{
public:
	GameState(void)					{};
	virtual ~GameState(void)		{};

	virtual void MouseUpdate(int X, int Y) = 0;

	virtual void Update(float fGameTime) = 0;
	virtual void Draw() = 0;

	virtual Level* GetLevel() = 0;

	virtual void AddActor(Actor* pActor) = 0;
	virtual PlayerCharacter* GetPlayer() = 0;
	//virtual kpuArrayList<Enemy*>*	GetEnemies() = 0;

	virtual bool HandleInputEvent(eInputEventType type, u32 button) = 0;
};

extern GameState* g_pGameState;

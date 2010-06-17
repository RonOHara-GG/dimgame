#pragma once
#include "GameState.h"
#include "Common/Utility/kpuArrayList.h"

class Level;
class Actor;
class PlayerCharacter;
enum eInputEventType;

class GameState_Default : public GameState
{
public:
	GameState_Default(void);
	virtual ~GameState_Default(void);

	virtual void MouseUpdate(int X, int Y);

	virtual void Update(float fGameTime);
	virtual void Draw();

	virtual Level*					GetLevel()		{ return m_pCurrentLevel; }
	virtual PlayerCharacter*		GetPlayer()		{ return m_pPlayer; }
	virtual kpuArrayList<Actor*>*	GetActors()		{ return m_paActors; }

	virtual void AddActor(Actor* pActor);

	virtual bool HandleInputEvent(eInputEventType type, u32 button);

protected:
	Level*					m_pCurrentLevel;

	PlayerCharacter*		m_pPlayer;
	kpuArrayList<Actor*>*	m_paActors;
};

#pragma once
#include "GameState.h"
#include "Common/Utility/kpuArrayList.h"

class Level;
class Actor;
class PlayerCharacter;
enum eInputEventType;

class GameState_GamePlay : public GameState
{
public:
	GameState_GamePlay(void);
	virtual ~GameState_GamePlay(void);

	virtual void MouseUpdate(int X, int Y);
	virtual void ScreenCordsToGameCords(kpuVector& vCords);

	virtual void Update(float fDeltaTime);
	virtual void Draw();

	virtual PlayerCharacter*		GetPlayer()		{ return m_pPlayer; }
	virtual kpuArrayList<Actor*>*	GetActors()		{ return m_paActors; }

	virtual void AddActor(Actor* pActor);

	virtual bool HandleInputEvent(eInputEventType type, u32 button);

protected:

	PlayerCharacter*		m_pPlayer;
	kpuArrayList<Actor*>*	m_paActors;
};

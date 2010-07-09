#pragma once
#include "Common/Utility/kpuLinkedList.h"
#include "Common/Utility/kpuMatrix.h"

class Level;
class Actor;
class PlayerCharacter;
class kpgUIManager;
class kpuCameraController;
class kpuVector;
enum eInputEventType;

class GameState
{
public:
	GameState(void)					{};
	virtual ~GameState(void)		{};

	virtual void MouseUpdate(int X, int Y) = 0;
	virtual void ScreenCordsToGameCords(kpuVector& vCords) = 0;

	virtual void Update(float fDeltaTime) = 0;
	virtual void Draw() = 0;

	virtual Level* GetLevel()		{ return m_pCurrentLevel; }
	virtual kpgUIManager* GetUIManager() { return m_pUIManager; }

	virtual void AddActor(Actor* pActor) = 0;
	virtual PlayerCharacter* GetPlayer() = 0;
	//virtual kpuArrayList<Enemy*>*	GetEnemies() = 0;

	virtual bool HandleInputEvent(eInputEventType type, u32 button) = 0;

protected:	
	Level*					m_pCurrentLevel;
	kpgUIManager*			m_pUIManager;
	kpuCameraController*	m_pCamera;
	kpuMatrix				m_mProjection;
};

extern GameState*		g_pGameState;
extern void ChangeGameState(GameState* newGameState);
extern void Terminate();

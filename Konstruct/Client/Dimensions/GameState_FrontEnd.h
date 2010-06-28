#pragma once
#include "GameState.h"
#include "Common/Utility/kpuArrayList.h"

class Level;
class Actor;
class PlayerCharacter;
enum eInputEventType;
enum ePlayerClass;

class GameState_FrontEnd : public GameState
{
public:
	GameState_FrontEnd(void);
	virtual ~GameState_FrontEnd(void);

	virtual void MouseUpdate(int X, int Y);
	virtual void ScreenCordsToGameCords(kpuVector& vCords) { }

	virtual void Update(float fGameTime);
	virtual void Draw();

	virtual Level*					GetLevel()		{ return m_pCurrentLevel; }
	virtual PlayerCharacter*		GetPlayer()		{ return m_pPlayer; }
	virtual kpuArrayList<Actor*>*	GetActors()		{ return m_paActors; }

	virtual void AddActor(Actor* pActor);

	virtual bool HandleInputEvent(eInputEventType type, u32 button);

protected:
	bool					LoadMostRecentSave();
	bool					LoadGame(const char* szFile);

	Level*					m_pCurrentLevel;

	PlayerCharacter*		m_pPlayer;
	kpuArrayList<Actor*>*	m_paActors;

	/*Character Creation*/	
	kpuLinkedList*			m_plCurrentModel;

	char					m_szName[MAX_NAME];
	ePlayerClass			m_eStartClass;
};

//Front End specific click events
#define CE_EXIT						0x7c84f21f	//Exit the program
#define CE_NEW_GAME					0x6c2925a9	//Start new game
#define CE_LOAD_SAVED_GAME			0xa6e8472	//Load the selected saved game
#define CE_LOAD_MOST_RECENT			0x34d4e6c9	//Load most recent save
#define CE_NEXT_PLAYER_MODEL		0x0
#define CE_SELECT_CLASS_BRAWLER		0x0
#define CE_SELECT_CLASS_ARCHER		0x0
#define CE_SELECT_CLASS_MEDIC		0x0
#define CE_SELECT_CLASS_PRIEST		0x0
#define CE_SELECT_CLASS_ROCKETEER	0x0
#define CE_SELECT_CLASS_SWORDSMAN	0x0
#define CE_SELECT_CLASS_OCCULIST	0x0
#define CE_SELECT_CLASS_MARKSMAN	0x0
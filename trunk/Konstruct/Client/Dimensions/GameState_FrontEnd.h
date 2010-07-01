#pragma once
#include "GameState.h"
#include "Common/Utility/kpuArrayList.h"
#include "Common/Utility/kpuMatrix.h"

class Level;
class Actor;
class PlayerCharacter;
class kpgModel;
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
	struct Planet
	{
		kpgModel*	m_pModel;
		Planet*		m_pParent;
		float		m_fRotationSpeed;
		float		m_fRotation;
		float		m_fOrbitSpeed;
		float		m_fOrbit;
		float		m_fDistance;
	};

	void					LoadBackground(const char* szFile);
	void					NextCharacterModel();
	void					PreviousCharacterModel();
	void					LoadAllPlayerModels(const char* szFile);
	bool					LoadMostRecentSave();
	bool					LoadGame(const char* szFile);

	Level*					m_pCurrentLevel;

	PlayerCharacter*		m_pPlayer;
	kpuArrayList<Actor*>*	m_paActors;

	kpuLinkedList			m_lBgObjects;
	Planet*					m_pLookAt;
	kpuMatrix				m_mBgProjection;
	kpuMatrix				m_mBgView;

	/*Character Creation*/	
	kpuMatrix				m_mCharacterView;
	bool					m_bCharacterCreation;
	kpuLinkedList			m_lPlayerModels;
	kpuLinkedList*			m_plCurrentModel;

	char					m_szName[MAX_NAME];
	ePlayerClass			m_eStartClass;
};

//Front End specific click events
#define CE_EXIT							0x7c84f21f	//Exit the program
#define CE_ENTER_GAME					0xB1248AFD	//Enter Game
#define CE_ENTER_CHARACTER_CREATION		0x129A1407
#define CE_LOAD_SAVED_GAME				0xa6e8472	//Load the selected saved game
#define CE_LOAD_MOST_RECENT				0x34d4e6c9	//Load most recent save
#define CE_NEXT_PLAYER_MODEL			0xBa1a75a2
#define CE_PREVIOUS_PLAYER_MODEL		0x99124100
#define CE_SELECT_BRAWLER				0x0
#define CE_SELECT_ARCHER				0x0
#define CE_SELECT_MEDIC					0x0
#define CE_SELECT_PRIEST				0x0
#define CE_SELECT_ROCKETEER				0x0
#define CE_SELECT_SWORDSMAN				0x0
#define CE_SELECT_OCCULIST				0x0
#define CE_SELECT_MARKSMAN				0x0


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

#define NUM_PLANETS				12
#define PLANET_EARTH			3
#define PLANET_SPACE_STATION	11

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
		kpgModel*	m_pModel;				// Pointer to the model in the level
		kpgModel*	m_pOrbit;				// Pointer to the planet this is orbiting
		float		m_fRotationalPeriod;	// Time in seconds it takes to complete one rotation around the axis
		float		m_fOrbitalPeriod;		// Time in seconds it takes to complete one orbit around the parent
		bool		m_bOrbitReverse;		// true if this should orbit in the reverse direction
		kpuVector	m_vOrbitAxis;
	};

	void					Orbit(int iPlanet, float fDeltaTime);
	void					SetupOrbitalData(const char* szFile);
	void					NextCharacterModel();
	void					PreviousCharacterModel();
	void					LoadAllPlayerModels(const char* szFile);
	bool					LoadMostRecentSave();
	bool					LoadGame(const char* szFile);

	Level*					m_pCurrentLevel;

	PlayerCharacter*		m_pPlayer;
	kpuArrayList<Actor*>*	m_paActors;

	kpuMatrix				m_mBgProjection;
	Planet					m_aPlanets[NUM_PLANETS];

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


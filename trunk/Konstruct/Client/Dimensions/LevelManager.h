#pragma once

class Level;

typedef enum _eLevelID
{
	eLID_FrontEnd,
	eLID_SpaceStation,

	eLID_Last
} eLevelID;

class LevelManager
{
public:
	LevelManager(void);
	~LevelManager(void);

	Level* LoadLevel(eLevelID eLevelToLoad);

protected:
	char*		m_aLevelFileNames[eLID_Last];
	Level*		m_pLevels[eLID_Last];

public:
	static LevelManager* GetInstance();
	static void Shutdown();
};

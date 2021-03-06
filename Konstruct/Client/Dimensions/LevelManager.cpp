#include "StdAfx.h"
#include "LevelManager.h"
#include "Level.h"
#include "Common/Utility/kpuXmlParser.h"

static LevelManager* g_pLevelManager = 0;

static const u32 s_uHash_ID = 0x005973F2;
static const u32 s_uHash_File = 0x7c853F05;

LevelManager::LevelManager(void)
{
	for( int i = 0; i < eLID_Last; i++ )
	{
		m_aLevelFileNames[i] = 0;
		m_pLevels[i] = 0;
	}

	// Load the level list
	kpuXmlParser* pParser = new kpuXmlParser();
	if(pParser->LoadFile("Assets/Levels.xml") )
	{
		while( pParser->HasElement() )
		{		
			if( !_strnicmp(pParser->GetValue(), "LevelList", 9) )
			{
				pParser->FirstChildElement();
				while( pParser->HasElement() )
				{				
					if( !_strnicmp(pParser->GetValue(), "Level", 5) )
					{					
						int iID = pParser->GetAttributeAsInt("ID");

						if( m_aLevelFileNames[iID] )
							free(m_aLevelFileNames[iID]);
						m_aLevelFileNames[iID] = _strdup(pParser->GetAttribute("File"));
					}
					pParser->NextSiblingElement();
				}
				pParser->Parent();
				break;
			}
			pParser->NextSiblingElement();
		}		
	}

	delete pParser;
}

LevelManager::~LevelManager(void)
{
	for( int i = 0; i < eLID_Last; i++ )
	{
		if( m_aLevelFileNames[i] )
			free(m_aLevelFileNames[i]);
		if( m_pLevels[i] )
			delete m_pLevels[i];
	}
}

LevelManager* LevelManager::GetInstance()
{
	if( !g_pLevelManager )
	{
		g_pLevelManager = new LevelManager();
	}
	return g_pLevelManager;
}

void LevelManager::Shutdown()
{
	if( g_pLevelManager )
	{
		delete g_pLevelManager;
		g_pLevelManager = 0;
	}
}

Level* LevelManager::LoadLevel(eLevelID eLevelToLoad)
{
	// Make sure the level index is valid
	if( eLevelToLoad >= 0 && eLevelToLoad < eLID_Last )
	{
		// Return an already loaded copy of the level
		if( m_pLevels[eLevelToLoad] )
		{
			// May possibly want to reload the level?
			return m_pLevels[eLevelToLoad];
		}

		// Level isnt loaded yet, make sure we have a file name
		if( m_aLevelFileNames[eLevelToLoad] )
		{
			// Load the level
			Level* pLevel = new Level();
			if( pLevel->Load(m_aLevelFileNames[eLevelToLoad]) )
			{
				m_pLevels[eLevelToLoad] = pLevel;
				return pLevel;
			}
		}
	}
	return 0;
}

#pragma once

#include "Common/Utility/kpuFixedArray.h"
#include "Common/Utility/kpuArrayList.h"
#include "LoadStructures.h"


class kpgModel;
class kpgLight;
class TerrainModel;
class kpgRenderer;
class kpuQuadTree;
class Grid;
class Actor;
class kpuXmlParser;
class Npc;

class Level
{
public:
	Level(void);	
	~Level(void);

	bool Load(const char* pszLevelFile);
	void GenerateEnemies(kpuArrayList<Actor*> *pActors);

	Grid*		GetGrid() const { return m_pLevelGrid; }
	
	kpuQuadTree* GetQuadTree() { return m_pQuadTree; }
	kpgModel*	FindModelByName(u32 iNameHash);

	void Update(float fDeltaTime);
	void Draw(kpgRenderer* pRenderer);

protected:
	void	LoadNpcs(kpuXmlParser* pParser);
	
	Grid*							m_pLevelGrid; //The tile grid of the current level
	kpuQuadTree*					m_pQuadTree;
	char						    m_szName[64];
	kpuFixedArray<Npc*>*			m_paNpcs;
	kpuFixedArray<kpgModel*>*		m_paModels;
	kpuFixedArray<kpgLight*>*		m_paLights;
	TerrainModel*					m_pTerrain;
	

	//void LoadEnemyType(const char* pszFile, kpuFixedArray<EnemyLoadStructure>* pArray);	
};

#pragma once

#include "Common/Utility/kpuFixedArray.h"
#include "Common/Utility/kpuArrayList.h"
#include "LoadStructures.h"


class kpgModel;
class TerrainModel;
class kpgRenderer;
class kpuQuadTree;
class Grid;
class Actor;

class Level
{
public:
	Level(void);	
	~Level(void);

	bool Load(const char* pszLevelFile);
	void GenerateEnemies(kpuArrayList<Actor*> *pActors);
	//void LoadEnemyList(kpuArrayList<Enemy*>* paEnemies);

	Grid*		GetGrid() const { return m_pLevelGrid; }
	kpgModel*	GetEnemyModel(int iIndex ) const { return (*m_paEnemyModels)[iIndex]; }
	kpuQuadTree* GetQuadTree() { return m_pQuadTree; }
	

	void Update();
	void Draw(kpgRenderer* pRenderer);

protected:
	
	Grid*							m_pLevelGrid; //The tile grid of the current level
	kpuQuadTree*					m_pQuadTree;
	char						    m_szName[64];
	TerrainModel*				m_pTerrain;
	kpuFixedArray<kpgModel*>*		m_paEnemyModels;

	//void LoadEnemyType(const char* pszFile, kpuFixedArray<EnemyLoadStructure>* pArray);
	
	
	
};

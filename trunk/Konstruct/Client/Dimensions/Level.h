#pragma once
#include "Common/Utility/kpuFixedArray.h"

class kpgModel;
class kpgRenderer;
class Grid;
class Enemy;

class Level
{
public:
	Level(void);	
	~Level(void);

	bool Load(const char* pszLevelFile);

	Grid* GetGrid() const { return m_pLevelGrid; }
	kpgModel* GetEnemyModel(int iIndex ) const { return (*m_paEnemyModels)[iIndex]; }

	void Update();
	void Draw(kpgRenderer* pRenderer);

protected:
	Grid*						m_pLevelGrid; //The tile grid of the current level
	char						m_szName[64];
	kpuFixedArray<kpgModel*>*	m_paModels;
	kpuFixedArray<kpgModel*>*	m_paEnemyModels;
};

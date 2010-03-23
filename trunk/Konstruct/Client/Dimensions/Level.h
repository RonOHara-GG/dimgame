#pragma once
#include "Common/Utility/kpuFixedArray.h"

class kpgModel;
class kpgRenderer;

class Level
{
public:
	Level(void);	
	~Level(void);

	bool Load(const char* pszLevelFile);

	void Update();
	void Draw(kpgRenderer* pRenderer);

protected:
	char						m_szName[64];
	kpuFixedArray<kpgModel*>*	m_paModels;
};

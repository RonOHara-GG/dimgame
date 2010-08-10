#pragma once
#include "Common/Utility/kpuMap.h"

class kpgAnimation;
class kpuXmlParser;

class kpgAnimationManager
{
public:
	kpgAnimationManager(void);
	~kpgAnimationManager(void);

	void LoadAnimation(const char* szFile);

protected:
	typedef struct _sNodeData
	{
		u32 uName;
		char* pData;
	}sNodeData;

	void LoadAnimationLibrary(kpuXmlParser* pParser);
	void LoadBoneIndicies(kpuXmlParser *pParser);

protected:
	kpuMap<u32, int>*			m_pBoneIndicieMap;
	kpuMap<u32, kpgAnimation*>*	m_pAnimations;
};

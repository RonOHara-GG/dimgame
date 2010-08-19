#pragma once
#include "Common/Utility/kpuMap.h"
#include "Common/Utility/kpuFixedArray.h"
#include "kpgAnimation.h"

class kpuXmlParser;
class kpgLinkedList;
class kpgAnimationInstance;

class kpgAnimationManager
{
public:
	kpgAnimationManager(void);
	~kpgAnimationManager(void);

	static kpgAnimationManager* kpgAnimationManager::GetInstance();

	void LoadAnimation(const char* szFile, u32 uName);
	kpgAnimationInstance*	GetNewAnimation(u32 uName);

protected:
	typedef struct _sNodeData
	{
		u32 uName;
		char* pData;
	}sNodeData;

	typedef struct _sBoneData
	{
		int iIndex;
		u32 uParent;	
		u32 uName;
		char* pFloats;
	}sBoneData;

	void LoadAnimationLibrary(kpuXmlParser* pParser, kpuFixedArray<kpgAnimation::sBone*>& transforms);
	void LoadSkeleton(kpuXmlParser* pParser);
	void LoadBone(kpuXmlParser* pParser, u32 uParent);
	void LoadBoneIndicies(kpuXmlParser *pParser);
	void CreateSkeletonMap(u32 uBone);

protected:
	static kpgAnimationManager* m_spAnimationManager;

	kpuMap<u32, sBoneData>*		m_pBoneIndicieMap;
	kpuMap<u32, kpgAnimation*>*	m_pAnimations;
	kpuLinkedList*				m_plSkeletonSources;
	kpuLinkedList*				m_plBoneHierarchy;
};

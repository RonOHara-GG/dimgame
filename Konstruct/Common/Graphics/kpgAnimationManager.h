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

	void LoadAnimationLibrary(kpuXmlParser* pParser, kpuFixedArray<kpgAnimation::sBone*>& bones);
	void LoadSkeleton(kpuXmlParser* pParser);
	void LoadBone(kpuXmlParser* pParser, u32 uParent);
	void LoadControllers(kpuXmlParser* pParser);
	void LoadInverseBinds(kpuXmlParser* pParser, kpuLinkedList* pSources);
	void LoadBoneIndicies(kpuXmlParser* pParser, kpuLinkedList* pSources);

	void CreateAnimation(u32 uName,kpuFixedArray<kpgAnimation::sBone*>* bones);
	void CreateSkeletonMap(u32 uBone);

protected:

	static kpgAnimationManager* m_spAnimationManager;

	kpuMap<u32, sBoneData>*		m_pBoneIndicieMap;
	kpuMap<u32, kpgAnimation*>*	m_pAnimations;
	kpuLinkedList*				m_plSkeletonSources;
	kpuLinkedList*				m_plBoneHierarchy;
	kpuFixedArray<kpuMatrix>*	m_paSkinningMatricies;
};

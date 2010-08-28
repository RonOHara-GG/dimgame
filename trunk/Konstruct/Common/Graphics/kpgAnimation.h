#pragma once
#include "Common/Utility/kpuFixedArray.h"
#include "Common/Utility/kpuMatrix.h"

class kpgAnimation
{
public:
	kpgAnimation(int iBoneCount);
	~kpgAnimation(void);

	typedef struct _sBone
	{
		u32							uName;
		kpuFixedArray<kpuMatrix>	aTransforms;
		kpuFixedArray<float>		aTimes;
		kpuMatrix					mBindPose;
		kpuMatrix					mInvBind;
		int							iParent;
	}sBone;

	int							BoneCount()					{ return  m_aBoneHierarchy.GetNumElements(); }
	int							GetBoneParent(int i)		{ return m_aBoneHierarchy[i]->iParent; }
	kpuFixedArray<float>*		GetTimes(int i)				{ return &m_aBoneHierarchy[i]->aTimes; }
	kpuFixedArray<kpuMatrix>*	GetTransforms(int i)		{ return &m_aBoneHierarchy[i]->aTransforms; }
	kpuMatrix&					GetBindPose(int i)			{ return m_aBoneHierarchy[i]->mBindPose; }
	kpuMatrix&					GetInvBind(int i)			{ return m_aBoneHierarchy[i]->mInvBind; }
	
	void						AddBone(int iIndex, sBone* pBone) {	m_aBoneHierarchy[iIndex] = pBone; }	

protected:
	u32							m_uName;
	kpuFixedArray<sBone*>		m_aBoneHierarchy;
	
};

#define MAX_BONES 32

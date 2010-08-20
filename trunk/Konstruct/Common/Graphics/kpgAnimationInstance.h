#pragma once
#include "Common/Utility/kpuFixedArray.h"
#include "Common/Utility/kpuMatrix.h"

class kpgAnimation;

class kpgAnimationInstance
{
public:
	kpgAnimationInstance(kpgAnimation* pAnimation);
	~kpgAnimationInstance(void);

	void SetBoneTransforms(kpuFixedArray<kpuMatrix>* paTransforms) { m_paOrigBoneMatricies = paTransforms; }
	bool Update(float fDeltaTime);
	kpuFixedArray<kpuMatrix>* GetTransformations() { return &m_aBoneTransformations; }

protected:
	kpgAnimation*				m_pAnimation;	
	float						m_fElaspedTime;
	kpuFixedArray<kpuMatrix>	m_aBoneTransformations;
	kpuFixedArray<kpuMatrix>*	m_paOrigBoneMatricies;
	
};

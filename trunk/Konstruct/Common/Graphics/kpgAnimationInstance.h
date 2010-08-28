#pragma once
#include "Common/Utility/kpuFixedArray.h"
#include "Common/Utility/kpuMatrix.h"

class kpgAnimation;

class kpgAnimationInstance
{
public:
	kpgAnimationInstance(kpgAnimation* pAnimation);
	~kpgAnimationInstance(void);
	
	bool Update(float fDeltaTime);
	kpuFixedArray<kpuMatrix>* GetTransformations() { return &m_aFinalTransformations; }

protected:
	kpgAnimation*				m_pAnimation;	
	float						m_fElaspedTime;
	bool						m_bPlaying;
	kpuFixedArray<kpuMatrix>	m_aBoneTransformations;	
	kpuFixedArray<kpuMatrix>	m_aFinalTransformations;
	
};

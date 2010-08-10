#pragma once
#include "Common/Utility/kpuFixedArray.h"
#include "Common/Utility/kpuMatrix.h"

class kpgAnimation;

class kpgAnimationInstance
{
public:
	kpgAnimationInstance(kpgAnimation* pAnimation);
	~kpgAnimationInstance(void);

	void Play();
	void Stop() { m_bPlaying = false; }
	void Update(float fDeltaTime);

protected:
	kpgAnimation*				m_pAnimation;
	int							m_iFrameCount;
	int							m_iCurrentFrame;
	float						m_fElaspedTime;
	float						m_fNextFrame;
	bool						m_bPlaying;
	kpuFixedArray<kpuMatrix>	m_aBoneTransformations;
	
};

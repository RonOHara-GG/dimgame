#pragma once
#include "Common/Utility/kpuFixedArray.h"
#include "Common/Utility/kpuMatrix.h"

class kpgAnimation
{
public:
	kpgAnimation(int iBoneCount, int iFrames);
	~kpgAnimation(void);

	int					BoneCount()					{ return m_aTransformations.GetNumElements() / GetFrameCount(); }
	int					GetFrameCount()				{ return m_aTransformations.GetNumElements(); } //Return the number of frames in the animation
	float				GetTime(int i)				{ return m_aTimes[i]; }
	const kpuMatrix&	GetTransformation(int i)	{ return m_aTransformations[i]; }

	void				SetTime(int i, float fVal)					{ m_aTimes[i] = fVal; }
	void				SetTransformation(int i, kpuMatrix& mMat)	{ m_aTransformations[i] = mMat; }
	

protected:
	u32							m_uName;
	kpuFixedArray<float>		m_aTimes;
	kpuFixedArray<kpuMatrix>	m_aTransformations;
};

#define MAX_BONES 32
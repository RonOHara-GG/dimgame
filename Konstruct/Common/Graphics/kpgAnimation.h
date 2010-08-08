#pragma once
#include "Common/Utility/kpuFixedArray.h"
#include "Common/Utility/kpuMatrix.h"

class kpgAnimation
{
public:
	kpgAnimation(void);
	~kpgAnimation(void);

	int					GetFrameCount()				{ return m_paTransformations->GetNumElements(); } //Return the number of frames in the animation
	float				GetTime(int i)				{ return (*m_paTimes)[i]; }
	const kpuMatrix&	GetTransformation(int i)	{ return (*m_paTransformations)[i]; }
	

protected:
	u32							m_uName;
	kpuFixedArray<float>*		m_paTimes;
	kpuFixedArray<kpuMatrix>*	m_paTransformations;
};

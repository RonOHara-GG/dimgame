#include "StdAfx.h"
#include "kpgAnimation.h"

kpgAnimation::kpgAnimation(int iBoneCount, int iFrames)
{
	m_aTimes.SetSize(iFrames);
	m_aTransformations.SetSize(iBoneCount);
}

kpgAnimation::~kpgAnimation(void)
{
}

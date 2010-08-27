#include "StdAfx.h"
#include "kpgAnimation.h"

kpgAnimation::kpgAnimation(int iBoneCount)
{
	m_aBoneHierarchy.SetSize(iBoneCount);
}

kpgAnimation::~kpgAnimation(void)
{
	for(int i = 0; i < m_aBoneHierarchy.GetNumElementsUsed(); i++)
	{
		delete m_aBoneHierarchy[i];
	}
}

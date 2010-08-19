#include "StdAfx.h"
#include "kpgAnimationInstance.h"
#include "kpgAnimation.h"

kpgAnimationInstance::kpgAnimationInstance(kpgAnimation* pAnimation)
{
	m_pAnimation = pAnimation;
	m_fElaspedTime = 0.0f;
	m_aBoneTransformations.SetSize(MAX_BONES);	
}

kpgAnimationInstance::~kpgAnimationInstance(void)
{
}

bool kpgAnimationInstance::Update(float fDeltaTime)
{	
	m_fElaspedTime += fDeltaTime;	
	bool bFinished = true;

	//get transformation
	for(int i = 0; i < m_pAnimation->BoneCount(); i++)
	{			
		kpuFixedArray<float>* pTimes = m_pAnimation->GetTimes(i);
		kpuFixedArray<kpuMatrix>* pTransforms = m_pAnimation->GetTransforms(i);

		int iCurrentKey = -1;
		float fNextKey = 0.0f;
		//see which key we are at
		for(int j = 0; j < pTimes->GetNumElementsUsed(); j++)
		{
			fNextKey = (*pTimes)[j];
			if( m_fElaspedTime <=  fNextKey )
			{
				iCurrentKey = j;
				bFinished = false;
				break;
			}
		}


		kpuMatrix mCurrent, mNext, mParent;
		mCurrent.Identity();
		mNext.Identity();

		if( iCurrentKey > -1 )
		{
			//get all the matricies
			mCurrent = (*pTransforms)[iCurrentKey++];		
			mNext = (*pTransforms)[iCurrentKey];			
		}

		int iParent = m_pAnimation->GetBoneParent(i);		
		mParent.Identity();
		if( iParent > -1 )
			m_aBoneTransformations[iParent];
		
		m_aBoneTransformations[i] = ( mCurrent + (mNext - mCurrent) * (m_fElaspedTime / fNextKey) ) * mParent;
	}	

	return !bFinished;
}

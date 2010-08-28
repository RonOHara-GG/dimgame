#include "StdAfx.h"
#include "kpgAnimationInstance.h"
#include "kpgAnimation.h"

kpgAnimationInstance::kpgAnimationInstance(kpgAnimation* pAnimation)
{
	m_pAnimation = pAnimation;
	m_fElaspedTime = 0.0f;
	m_bPlaying = true;
	m_aBoneTransformations.SetSize(MAX_BONES);	
	m_aFinalTransformations.SetSize(MAX_BONES);
}

kpgAnimationInstance::~kpgAnimationInstance(void)
{
}

bool kpgAnimationInstance::Update(float fDeltaTime)
{	
	if( !m_bPlaying )
		return false;

	m_fElaspedTime += fDeltaTime;	
	bool bFinished = true;

	//get transformation
	for(int i = 0; i < m_pAnimation->BoneCount(); i++)
	{			
		kpuFixedArray<float>* pTimes = m_pAnimation->GetTimes(i);
		kpuFixedArray<kpuMatrix>* pTransforms = m_pAnimation->GetTransforms(i);

		kpuMatrix mCurrent, mPrev, mParent;
		mCurrent = m_pAnimation->GetBindPose(i);
		mPrev.Identity();
		mParent.Identity();		

		//See if this bone has any transformations
		if( pTransforms->GetNumElementsUsed() )
		{
			int iCurrentKey = 0;
			float fNextKey = 0.0f;
			float fPrevKey = 0.0f;

			//see which key we are at
			for(int j = 0; j < pTimes->GetNumElementsUsed(); j++)
			{
				fNextKey = (*pTimes)[j];
				if( m_fElaspedTime <  fNextKey )
				{
					iCurrentKey = j;
					bFinished = false;
					break;
				}
				fPrevKey = fNextKey;
			}	

			//if the animation is not finished interpolate to the current matrix
			if( !bFinished )
			{
				mCurrent = (*pTransforms)[iCurrentKey];	
				if( iCurrentKey > 0 )
				{
					//get all the matricies					
					mPrev = (*pTransforms)[iCurrentKey - 1];
					float fTime = (m_fElaspedTime - fPrevKey) / (fNextKey - fPrevKey);

					mCurrent = mPrev + ( (mCurrent - mPrev) * fTime);
				}
				m_bPlaying = true;
			}
			else
			{
				//last key of animation
				mCurrent = (*pTransforms)[pTransforms->GetNumElementsUsed() - 1];	
				m_bPlaying = false;
			}
		}	

		//try and find the parent matrix
		int iParent = m_pAnimation->GetBoneParent(i);	
		if( iParent > -1 )
			mParent = m_aBoneTransformations[iParent];
		
		m_aBoneTransformations[i] = mCurrent * mParent;		
		m_aFinalTransformations[i] = m_pAnimation->GetInvBind(i) * m_aBoneTransformations[i];
	}	

	return true;
}
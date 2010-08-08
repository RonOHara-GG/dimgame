#include "StdAfx.h"
#include "kpgAnimationInstance.h"
#include "kpgAnimation.h"

kpgAnimationInstance::kpgAnimationInstance(kpgAnimation* pAnimation)
{
	m_pAnimation = pAnimation;
	m_fNextFrame = 0.0f;
	m_fElaspedTime = 0.0f;
	m_iCurrentFrame = 0;
	m_bPlaying = false;
	m_iFrameCount = m_pAnimation->GetFrameCount();
}

kpgAnimationInstance::~kpgAnimationInstance(void)
{
}

void kpgAnimationInstance::Play()
{
	//Make sure the animation isn't already playing
	if( !m_bPlaying )
	{
		//reset times and frame count
		m_iCurrentFrame = 0;
		m_fElaspedTime = 0.0f;
		m_fNextFrame = m_pAnimation->GetTime(m_iCurrentFrame);
		m_bPlaying = true;
	}
}

void kpgAnimationInstance::Update(float fDeltaTime)
{
	if( m_bPlaying )
	{
		m_fElaspedTime += fDeltaTime;

		if( m_fElaspedTime >= m_fNextFrame )
		{
			//Get the next frame
			m_iCurrentFrame++;

			m_iCurrentFrame = (m_iCurrentFrame > m_iFrameCount) ? 0 : m_iCurrentFrame;		
			m_fNextFrame = m_pAnimation->GetTime(m_iCurrentFrame);
		}
	}
}


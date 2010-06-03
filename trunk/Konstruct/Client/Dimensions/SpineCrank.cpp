#include "StdAfx.h"
#include "SpineCrank.h"

SpineCrank::SpineCrank(void)
{
	m_fBaseSpeed = 0.0f;
}

SpineCrank::~SpineCrank(void)
{
}

bool SpineCrank::Activate(PlayerCharacter *pSkillOwner)
{
	if( Strike::Activate(pSkillOwner) )
	{
		m_fSpeed = m_fCurrentSpeed + (pSkillOwner->GetStr() / 50 );
		return true;
	}

	return false;
}

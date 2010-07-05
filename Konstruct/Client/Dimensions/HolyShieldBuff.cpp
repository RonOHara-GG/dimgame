#include "StdAfx.h"
#include "HolyShieldBuff.h"
#include "PlayerCharacter.h"

HolyShieldBuff::HolyShieldBuff(Actor* pSource, Actor* pSpreadee, float fRadius, int iDmgReduction)
{
	m_pSource = pSource;
	m_fRadius = fRadius;
	m_iDamageReduction = iDmgReduction;

	pSpreadee->DamageReduction(m_iDamageReduction);

}

HolyShieldBuff::~HolyShieldBuff(void)
{
}

bool HolyShieldBuff::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	//see if still in range of target, if not remove buff
	if( kpuVector::DistanceSquared(pSkillOwner->GetLocation(), m_pSource->GetLocation()) > m_fRadius * m_fRadius )
	{
		pSkillOwner->DamageReduction(-m_iDamageReduction);
		return false;
	}

	return true;
}
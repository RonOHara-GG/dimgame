#include "StdAfx.h"
#include "PersistentSkill.h"

PersistentSkill::PersistentSkill(void)
{
	m_pTarget = 0;
}

PersistentSkill::PersistentSkill(Actor* pTarget)
{
	m_pTarget = pTarget;
}

PersistentSkill::~PersistentSkill(void)
{
	Deactivate();
}

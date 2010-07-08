#include "StdAfx.h"
#include "Npc.h"
#include "AIControl.h"

Npc::Npc(void):Actor()
{
	m_pAIBehavior = 0;
	m_pszName = 0;
	SetFlag(NPC);
}

Npc::~Npc(void)
{
	if( m_pAIBehavior )
		delete m_pAIBehavior;
}

void Npc::SetAI(AIControl* pAI)
{
	if( m_pAIBehavior )
		delete m_pAIBehavior;

	m_pAIBehavior = pAI;
}

bool Npc::Update(float fGameTime)
{
	if( !m_bStatic )
		UpdateMovement(fGameTime);

	if( m_pAIBehavior )
		m_pAIBehavior->Update(fGameTime);

	return true;
}




#include "StdAfx.h"
#include "NPC.h"
#include "AIControl.h"

NPC::NPC(void):Actor()
{
	m_pAIBehavior = 0;
}

NPC::~NPC(void)
{
	if( m_pAIBehavior )
		delete m_pAIBehavior;
}





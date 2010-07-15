#include "StdAfx.h"
#include "Npc.h"
#include "AIControl.h"
#include "External/tinyxml/tinyxml.h"
#include "Common/Utility/kpuFileManager.h"
#include "Common/Graphics/kpgModel.h"

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

void Npc::Load(TiXmlElement *pElement, kpgModel* pModel)
{
	
	m_pszName = _strdup(pElement->Attribute("Name"));
	
	m_pModel = pModel;

	//Get collision model
	const char* szModel = pElement->Attribute("Collision");
	kpgModel* pCollisionModel = 0;

	if( szModel )	
	{	
		pCollisionModel = new kpgModel();
		pCollisionModel->Load(szModel);
		CalculateBoundingVolumes(pCollisionModel);
		delete pCollisionModel;		
	}

	m_fActionRange = (float)atof(pElement->Attribute("ActionRange"));
}




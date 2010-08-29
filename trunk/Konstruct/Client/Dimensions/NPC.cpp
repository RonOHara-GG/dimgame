#include "StdAfx.h"
#include "Npc.h"
#include "AIControl.h"
#include "Common/Utility/kpuXmlParser.h"
#include "Common/Graphics/kpgModel.h"
#include "Common/Graphics/kpgGeometryInstance.h"

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

void Npc::Load(kpuXmlParser* pParser, kpgModel* pModel)
{
	
	m_pszName = _strdup(pParser->GetAttribute("Name"));
	
	if( m_pModel)
		delete m_pModel;

	m_pModel = new kpgModel();
	kpgGeometryInstance* pInst = pModel->GetInstance(0);
	m_pModel->SetGeometryInstance(pInst, pInst->GetMatrix());

	//Get collision model
	const char* szModel = pParser->GetAttribute("Collision");
	kpgModel* pCollisionModel = 0;

	if( szModel )	
	{	
		pCollisionModel = new kpgModel();
		pCollisionModel->Load(szModel);
		CalculateBoundingVolumes(pCollisionModel);
		delete pCollisionModel;		
	}

	m_fActionRange = pParser->GetAttributeAsFloat("ActionRange");
}




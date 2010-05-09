#include "StdAfx.h"
#include "kpuPhysicalObject.h"
#include "Common/Graphics/kpgModel.h"
#include "Common/Utility/kpuQuadTree.h"
#include "Common/Graphics/kpgGeometryInstance.h"

kpuPhysicalObject::kpuPhysicalObject(void)
{
	

}

kpuPhysicalObject::kpuPhysicalObject(kpuVector vMin, kpuVector vMax)
{
	Init(vMin, vMax);
}

kpuPhysicalObject::~kpuPhysicalObject(void)
{

}
void kpuPhysicalObject::SetPrimatives(kpuArrayList<kpuBoundingVolume>& aVolumes)
{
	for(int i = 0; i < aVolumes.Count(); i++ )
	{
		m_aCollisionPrimatives.Add(aVolumes[i]);
	}

}

void kpuPhysicalObject::SetLocation(const kpuVector& vNewLoc)
{
	//move bounding objects
	/*m_bBox.Move(vNewLoc - GetLocation());
	m_bSphere.Move(vNewLoc - GetLocation());*/
	//m_pCurrentNode->ReAdd(this);
	if( m_pModel )
	{
		m_pModel->GetInstance(0)->SetPosition(vNewLoc.GetX(), vNewLoc.GetY(), vNewLoc.GetZ());
	}	
}

kpuVector kpuPhysicalObject::GetLocation()
{
	if( m_pModel )
	{
		return m_pModel->GetInstance(0)->GetMatrix().GetD();
	}
	else
	{
		return kpuVector();
	}
}

const kpuMatrix& kpuPhysicalObject::GetMatrix()
{
	return m_pModel->GetInstance(0)->GetMatrix();
}

void kpuPhysicalObject::Init(kpuVector vMin, kpuVector vMax)
{
	m_bBox = kpuBoundingBox(vMin, vMax);

	//find longest side for radius
	float fRadius = (vMax - vMin).Length();	

	kpuVector vCenter = (vMax - vMin ) * 0.5 + vMin;

	m_bSphere = kpuBoundingSphere(fRadius / 2, vCenter);
}
void kpuPhysicalObject::Move(kpuVector& vVel)
{
	m_pCurrentNode->Remove(this);

	float fPercent = m_pCurrentNode->Move(vVel, this);
	vVel *= fPercent;

	
}


#include "StdAfx.h"
#include "kpuPhysicalObject.h"
#include "Common/Graphics/kpgModel.h"
#include "Common/Utility/kpuQuadTree.h"

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
	//create temp collision primatives
	kpuBoundingBox tempBox = m_bBox;
	kpuBoundingSphere tempSphere = m_bSphere;

	m_bBox = kpuBoundingBox(m_bBox);
	m_bSphere = kpuBoundingSphere(m_bSphere);

	//moving collision primatives and see if we collide with anything
	m_bSphere.Move(vVel);
	m_bBox.Move(vVel);

	float fPercent = m_pCurrentNode->Move(vVel, this);
	vVel *= fPercent;

	m_bBox = tempBox;
	m_bSphere = tempSphere;

	////move the stuff needed to move
	//m_bSphere.Move(vVel);
	//m_bBox.Move(vVel);

	
	//m_pCollisionModel->SetPosition(m_pCollisionModel->GetPosition() + vVel);

}

void kpuPhysicalObject::HandleCollision(kpuPhysicalObject* pCollision, float fDist, kpuVector& vVel)
{
	//move bounding objects back
	m_bSphere.Move(-vVel);
	m_bBox.Move(-vVel);

	//find new velocity due to collision
	//for now we will just not move
	vVel = kpuv_Zero;
}
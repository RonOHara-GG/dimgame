#include "StdAfx.h"
#include "kpuBoundingBox.h"
#include "kpuBoundingSphere.h"
#include "kpuBoundingCapsule.h"
#include "kpuCollisionDetection.h"


kpuBoundingBox::kpuBoundingBox(kpuVector vMin ,kpuVector vMax)
{
	m_vMax = vMax;
	m_vMin = vMin;
	m_eType = eVT_Box;
}

kpuBoundingBox::kpuBoundingBox(const kpuBoundingBox& bBox)
{
	m_vMax = bBox.m_vMax;
	m_vMin = bBox.m_vMin;
	m_eType = eVT_Box;
}

kpuBoundingBox::~kpuBoundingBox(void)
{
}

void kpuBoundingBox::operator =(const kpuBoundingBox& bBox)
{
	m_vMax = bBox.m_vMax;
	m_vMin = bBox.m_vMin;
}

kpuCollisionData kpuBoundingBox::Intersects(kpuBoundingVolume &bOther)
{
	kpuCollisionData collisionData;

	switch ( bOther.GetType() )
	{
	case eVT_Sphere:
		{
			kpuBoundingSphere* pSphere = (kpuBoundingSphere*)&bOther;
			return kpuCollisionDetection::SphereVsBox(pSphere->GetLocation(), pSphere->GetRadius(),m_vMin, m_vMax); 
		}
	case eVT_Box:
		{
			kpuBoundingBox* pBox = (kpuBoundingBox*)&bOther;
			return kpuCollisionDetection::BoxVsBox(m_vMin, m_vMax, pBox->m_vMin, pBox->m_vMax);
		}
	case eVT_Capsule:
		{
			kpuBoundingCapsule* pCapsule = (kpuBoundingCapsule*)&bOther;
			return kpuCollisionDetection::BoxVsCapsule(m_vMin, m_vMax, pCapsule->GetStart(), pCapsule->GetEnd(), pCapsule->GetRadius());
		}
	}

	return collisionData;

}

void kpuBoundingBox::Transform(const kpuMatrix &matrix)
{
	m_vMin *= matrix;
	m_vMax *= matrix;
}

//bool kpuBoundingBox::Intersects(kpuBoundingBox &bBox)
//{
//	if(m_vMin.GetX() >= bBox.m_vMax.GetX() )
//		return false;
//	if(m_vMax.GetX() <= bBox.m_vMin.GetX() )
//		return false;
//
//	if(m_vMin.GetY() >= bBox.m_vMax.GetY() )
//		return false;
//	if(m_vMax.GetY() <= bBox.m_vMin.GetY() )
//		return false;
//
//	if(m_vMin.GetZ() >= bBox.m_vMax.GetZ() )
//		return false;
//	if(m_vMax.GetZ() <= bBox.m_vMin.GetZ() )
//		return false;
//	
//			
//	return true;
//}

bool kpuBoundingBox::Contains2D(kpuBoundingBox& bBox)
{
	//if the this min is less than bbox min and bbox min is less than max and max is greater than bbox max and bbox max is greater than this min then
	//bbox is inside of this box
	if ( m_vMin.GetX() <= bBox.GetMin().GetX() && m_vMin.GetZ() <= bBox.GetMin().GetZ() &&
		m_vMax.GetX() >= bBox.GetMin().GetX() && m_vMax.GetZ() >= bBox.GetMin().GetZ() )
	{
		if ( m_vMin.GetX() <= bBox.GetMax().GetX() && m_vMin.GetZ() <= bBox.GetMax().GetZ() &&
		m_vMax.GetX() >= bBox.GetMax().GetX() && m_vMax.GetZ() >= bBox.GetMax().GetZ() )
		{
			return true;
		}
	}


	return false;
}

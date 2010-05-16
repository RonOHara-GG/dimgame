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

void kpuBoundingBox::Intersects(kpuBoundingVolume* bOther, const kpuMatrix& matrix, kpuCollisionData& data)
{
	switch ( bOther->GetType() )
	{
	case eVT_Sphere:
		{
			kpuBoundingSphere pSphere = *(kpuBoundingSphere*)bOther;
			pSphere.Transform(matrix);
			kpuCollisionDetection::SphereVsBox(pSphere.GetLocation(), pSphere.GetRadius(), m_vMin, m_vMax, data); 
			break;
		}
	case eVT_Box:
		{
			kpuBoundingBox pBox = *(kpuBoundingBox*)bOther;
			pBox.Transform(matrix);
			kpuCollisionDetection::BoxVsBox(m_vMin, m_vMax, pBox.m_vMin, pBox.m_vMax, data);
			break;
		}
	case eVT_Capsule:
		{
			kpuBoundingCapsule pCapsule = *(kpuBoundingCapsule*)bOther;
			pCapsule.Transform(matrix);
			kpuCollisionDetection::BoxVsCapsule(m_vMin, m_vMax, pCapsule.GetStart(), pCapsule.GetEnd(), pCapsule.GetRadius(), data);
			break;
		}
	}

}

void kpuBoundingBox::Transform(const kpuMatrix &matrix)
{
	float fWidth = m_vMax.GetX() - m_vMin.GetX();
	float fHeight = m_vMax.GetY() - m_vMin.GetY();
	float fLength = m_vMax.GetZ() - m_vMin.GetZ();

	//rotate min and max
	kpuVector transformMin = m_vMin * matrix;
	kpuVector transformMax = m_vMax * matrix;

	//find new min and max
	if( transformMin.GetX() > transformMax.GetX() )
	{	
		float fMin = transformMax.GetX();
		transformMax.SetX(transformMin.GetX());
		transformMin.SetX(fMin);
	}

	if( transformMin.GetY() > transformMax.GetY() )
	{	
		float fMin = transformMax.GetY();
		transformMax.SetY(transformMin.GetY());
		transformMin.SetY(fMin);
	}

	if( transformMin.GetZ() > transformMax.GetZ() )
	{	
		float fMin = transformMax.GetZ();
		transformMax.SetZ(transformMin.GetZ());
		transformMin.SetZ(fMin);
	}

	m_vMax = transformMax;
	m_vMin = transformMin;

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


bool kpuBoundingBox::Contains2D(kpuBoundingCapsule &bCapsule)
{
	bool bCollided = false;
	if( bCapsule.GetStart().GetX() > m_vMax.GetX() )
	{
		//then the capsule end must be less than this boxes max x
		bCollided = bCapsule.GetEnd().GetX() - bCapsule.GetRadius() < m_vMax.GetX();

	}

	if( bCapsule.GetStart().GetX() < m_vMin.GetX() )
	{
		//then the capsule end must be greater than this boxes min x
		bCollided = bCapsule.GetEnd().GetX() + bCapsule.GetRadius() > m_vMin.GetX();
	}

	if( bCapsule.GetStart().GetZ() > m_vMax.GetZ() )
	{
		//then the capsule end must be less than this boxes max z
		bCollided = bCapsule.GetEnd().GetZ() - bCapsule.GetRadius() < m_vMax.GetZ();

	}

	if( bCapsule.GetStart().GetZ() < m_vMin.GetZ() )
	{
		//then the capsule end must be greater than this boxes min z
		bCollided = bCapsule.GetEnd().GetZ() + bCapsule.GetRadius() > m_vMin.GetZ();
	}

	//or the box contains the start or end
	if( bCapsule.GetStart().GetX() > m_vMin.GetX() && bCapsule.GetStart().GetX() < m_vMax.GetX() && bCapsule.GetStart().GetZ() > m_vMin.GetZ() && bCapsule.GetStart().GetZ() < m_vMax.GetZ() )
		bCollided = true;

	if( bCapsule.GetEnd().GetX() > m_vMin.GetX() && bCapsule.GetEnd().GetX() < m_vMax.GetX() && bCapsule.GetEnd().GetZ() > m_vMin.GetZ() && bCapsule.GetEnd().GetZ() < m_vMax.GetZ() )
		bCollided = true;

	return bCollided;
}
#include "StdAfx.h"
#include "kpuBoundingBox.h"
#include "kpuBoundingSphere.h"
#include "kpuBoundingCapsule.h"
#include "kpuCollisionDetection.h"


kpuBoundingBox::kpuBoundingBox(kpuVector vMin ,kpuVector vMax)
{
	m_vOffset = (vMax - vMin ) * 0.5;
	m_vCenter = vMin + m_vOffset;	

	m_eType = eVT_Box;
}

kpuBoundingBox::kpuBoundingBox(const kpuBoundingBox& bBox)
{
	m_vCenter = bBox.m_vCenter;
	m_vOffset = bBox.m_vOffset;
	m_eType = eVT_Box;
}

kpuBoundingBox::~kpuBoundingBox(void)
{
}

void kpuBoundingBox::operator =(const kpuBoundingBox& bBox)
{
	m_vCenter = bBox.m_vCenter;
	m_vOffset = bBox.m_vOffset;
}

void kpuBoundingBox::Intersects(kpuBoundingVolume* bOther, const kpuMatrix& matrix, kpuCollisionData& data)
{
	switch ( bOther->GetType() )
	{
	case eVT_Sphere:
		{
			kpuBoundingSphere pSphere = *(kpuBoundingSphere*)bOther;
			pSphere.Transform(matrix);
			kpuCollisionDetection::SphereVsBox(pSphere.GetLocation(), pSphere.GetRadius(), m_vCenter - m_vOffset, m_vCenter + m_vOffset, data); 
			break;
		}
	case eVT_Box:
		{
			kpuBoundingBox pBox = *(kpuBoundingBox*)bOther;
			pBox.Transform(matrix);
			kpuCollisionDetection::BoxVsBox(m_vCenter - m_vOffset, m_vCenter + m_vOffset, pBox.m_vCenter - pBox.m_vOffset, pBox.m_vCenter + pBox.m_vOffset, data);
			break;
		}
	case eVT_Capsule:
		{
			kpuBoundingCapsule pCapsule = *(kpuBoundingCapsule*)bOther;
			pCapsule.Transform(matrix);
			kpuCollisionDetection::BoxVsCapsule(m_vCenter , m_vOffset, pCapsule.GetStart(), pCapsule.GetEnd(), pCapsule.GetRadius(), data);
			break;
		}
	}

}

void kpuBoundingBox::Transform(const kpuMatrix &matrix)
{
	//rotate min and max
	m_vCenter *= matrix;
	kpuVector transformOffset = m_vOffset * matrix;

	kpuVector vMax = m_vCenter + transformOffset;
	kpuVector vMin = m_vCenter - transformOffset;

	//find new min and max
	if( vMin.GetX() > vMax.GetX() )
	{	
		float fMin = vMax.GetX();
		vMax.SetX(vMin.GetX());
		vMin.SetX(fMin);
	}

	if( vMin.GetY() >vMax.GetY() )
	{	
		float fMin = vMax.GetY();
		vMax.SetY(vMin.GetY());
		vMin.SetY(fMin);
	}

	if( vMin.GetZ() > vMax.GetZ() )
	{	
		float fMin = vMax.GetZ();
		vMax.SetZ(vMin.GetZ());
		vMin.SetZ(fMin);
	}

	m_vOffset = (vMax - vMin ) * 0.5;
}

bool kpuBoundingBox::Contains2D(kpuBoundingSphere& sphere)
{
	kpuVector vMax = m_vCenter + m_vOffset;
	kpuVector vMin = m_vCenter - m_vOffset;

	if( sphere.GetLocation().GetX() + sphere.GetRadius() > vMin.GetX() && sphere.GetLocation().GetX()- sphere.GetRadius() < vMax.GetX()  && sphere.GetLocation().GetZ()+ sphere.GetRadius() > vMin.GetZ() && sphere.GetLocation().GetZ() -  sphere.GetRadius() < vMax.GetZ() )
		return true;

	return false;
}

bool kpuBoundingBox::Contains2D(kpuBoundingVolume &bVolume)
{
	switch(bVolume.GetType() )
	{
	case kpuBoundingVolume::eVT_Box:
		{
			kpuBoundingBox* pBox = (kpuBoundingBox*)&bVolume;
			return Contains2D(*pBox);
		}
	case kpuBoundingVolume::eVT_Capsule:
		{
			kpuBoundingCapsule* pCapsule = (kpuBoundingCapsule*)&bVolume;
			return Contains2D(*pCapsule);
		}
	case kpuBoundingVolume::eVT_Sphere:
		{
			kpuBoundingSphere* pSphere = (kpuBoundingSphere*)&bVolume;
			return Contains2D(*pSphere);
		}
	}
}
bool kpuBoundingBox::Contains2D(kpuBoundingBox& bBox)
{
	//if the this min is less than bbox min and bbox min is less than max and max is greater than bbox max and bbox max is greater than this min then
	//bbox is inside of this box
	kpuVector vMax = m_vCenter + m_vOffset;
	kpuVector vMin = m_vCenter - m_vOffset;

	if ( vMin.GetX() <= bBox.GetMin().GetX() && vMin.GetZ() <= bBox.GetMin().GetZ() &&
		vMax.GetX() >= bBox.GetMin().GetX() && vMax.GetZ() >= bBox.GetMin().GetZ() )
	{
		if ( vMin.GetX() <= bBox.GetMax().GetX() && vMin.GetZ() <= bBox.GetMax().GetZ() &&
		vMax.GetX() >= bBox.GetMax().GetX() && vMax.GetZ() >= bBox.GetMax().GetZ() )
		{
			return true;
		}
	}


	return false;
}


bool kpuBoundingBox::Contains2D(kpuBoundingCapsule &bCapsule)
{
	kpuVector vMax = m_vCenter + m_vOffset;
	kpuVector vMin = m_vCenter - m_vOffset;

	bool bCollided = false;
	if( bCapsule.GetStart().GetX() > vMax.GetX() )
	{
		//then the capsule end must be less than this boxes max x
		bCollided = bCapsule.GetEnd().GetX() - bCapsule.GetRadius() < vMax.GetX();

	}

	if( bCapsule.GetStart().GetX() < vMin.GetX() )
	{
		//then the capsule end must be greater than this boxes min x
		bCollided = bCapsule.GetEnd().GetX() + bCapsule.GetRadius() > vMin.GetX();
	}

	if( bCapsule.GetStart().GetZ() > vMax.GetZ() )
	{
		//then the capsule end must be less than this boxes max z
		bCollided = bCapsule.GetEnd().GetZ() - bCapsule.GetRadius() < vMax.GetZ();

	}

	if( bCapsule.GetStart().GetZ() < vMin.GetZ() )
	{
		//then the capsule end must be greater than this boxes min z
		bCollided = bCapsule.GetEnd().GetZ() + bCapsule.GetRadius() > vMin.GetZ();
	}

	//or the box contains the start or end
	if( bCapsule.GetStart().GetX() > vMin.GetX() && bCapsule.GetStart().GetX() < vMax.GetX() && bCapsule.GetStart().GetZ() > vMin.GetZ() && bCapsule.GetStart().GetZ() < vMax.GetZ() )
		bCollided = true;

	if( bCapsule.GetEnd().GetX() > vMin.GetX() && bCapsule.GetEnd().GetX() < vMax.GetX() && bCapsule.GetEnd().GetZ() > vMin.GetZ() && bCapsule.GetEnd().GetZ() < vMax.GetZ() )
		bCollided = true;

	return bCollided;
}
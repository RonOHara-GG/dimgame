#include "StdAfx.h"
#include "kpuBoundingCapsule.h"
#include "kpuBoundingBox.h"
#include "kpuBoundingSphere.h"
#include "kpuCollisionDetection.h"

kpuBoundingCapsule::kpuBoundingCapsule(kpuVector v1, kpuVector v2, float fRadius)
{
	m_vStart = v1;
	m_vEnd = v2;
	m_fRadius = fRadius;
	m_eType = eVT_Capsule;
}

kpuBoundingCapsule::~kpuBoundingCapsule(void)
{
}

void kpuBoundingCapsule::Intersects(kpuBoundingVolume* bOther, const kpuMatrix& matrix, kpuCollisionData& data)
{
	switch ( bOther->GetType() )
	{
	case eVT_Sphere:
		{
			kpuBoundingSphere pSphere = *(kpuBoundingSphere*)bOther;
			pSphere.Transform(matrix);
			kpuCollisionDetection::SphereVsCapsule(pSphere.GetLocation(), pSphere.GetRadius(), m_vStart, m_vEnd, m_fRadius, data);
			break;
		}
	case eVT_Box:
		{
			kpuBoundingBox pBox = *(kpuBoundingBox*)bOther;
			pBox.Transform(matrix);
			kpuCollisionDetection::BoxVsCapsule(pBox.GetMin(), pBox.GetMax(),m_vStart, m_vEnd, m_fRadius, data);
			break;
		}
	case eVT_Capsule:
		{
			kpuBoundingCapsule pCapsule = *(kpuBoundingCapsule*)bOther;
			pCapsule.Transform(matrix);
			kpuCollisionDetection::CapsuleVsCapsule(m_vStart, m_vEnd, m_fRadius, pCapsule.GetStart(), pCapsule.GetEnd(), pCapsule.GetRadius(), data);
			break;
		}
	}

}

void kpuBoundingCapsule::Transform(const kpuMatrix& matrix)
{
	m_vStart *= matrix;
	m_vEnd *= matrix;
}
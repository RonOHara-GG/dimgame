#include "StdAfx.h"
#include "kpuBoundingCapsule.h"
#include "kpuBoundingBox.h"
#include "kpuBoundingSphere.h"
#include "kpuCollisionDetection.h"

kpuBoundingCapsule::kpuBoundingCapsule(kpuVector v1, kpuVector v2, float fRadius)
{
}

kpuBoundingCapsule::~kpuBoundingCapsule(void)
{
}

kpuCollisionData kpuBoundingCapsule::Intersects(kpuBoundingVolume &bOther)
{
	kpuCollisionData collisionData;

	switch ( bOther.GetType() )
	{
	case eVT_Sphere:
		{
			kpuBoundingSphere* pSphere = (kpuBoundingSphere*)&bOther;
			return kpuCollisionDetection::SphereVsCapsule(pSphere->GetLocation(), pSphere->GetRadius(), m_vStart, m_vEnd, m_fRadius);
		}
	case eVT_Box:
		{
			kpuBoundingBox* pBox = (kpuBoundingBox*)&bOther;
			return kpuCollisionDetection::BoxVsCapsule(pBox->GetMin(), pBox->GetMax(),m_vStart, m_vEnd, m_fRadius);
		}
	case eVT_Capsule:
		{
			kpuBoundingCapsule* pCapsule = (kpuBoundingCapsule*)&bOther;
			return kpuCollisionDetection::CapsuleVsCapsule(m_vStart, m_vEnd, m_fRadius, pCapsule->GetStart(), pCapsule->GetEnd(), pCapsule->GetRadius());
		}
	}

	return collisionData;

}

void kpuBoundingCapsule::Transform(const kpuMatrix& matrix)
{
	m_vStart *= matrix;
	m_vEnd *= matrix;
}
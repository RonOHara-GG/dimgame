#include "StdAfx.h"
#include "kpuBoundingSphere.h"
#include "kpuBoundingBox.h"
#include "kpuBoundingCapsule.h"
#include "kpuCollisionDetection.h"


kpuBoundingSphere::kpuBoundingSphere(float fRadius, kpuVector vLoc)
{
	m_fRadius = fRadius;
	m_vLocation = vLoc;
	m_eType = eVT_Sphere;
}

kpuBoundingSphere::kpuBoundingSphere(const kpuBoundingSphere& bSphere)
{
	m_fRadius = bSphere.m_fRadius;
	m_vLocation = bSphere.m_vLocation;
	m_eType = eVT_Sphere;

}
kpuBoundingSphere::~kpuBoundingSphere(void)
{
}

void kpuBoundingSphere::operator =(const kpuBoundingSphere& bSphere)
{
	m_fRadius = bSphere.m_fRadius;
	m_vLocation = bSphere.m_vLocation;
}

kpuCollisionData kpuBoundingSphere::Intersects(kpuBoundingVolume &bOther)
{
	kpuCollisionData collisionData;

	switch ( bOther.GetType() )
	{
	case eVT_Sphere:
		{
			kpuBoundingSphere* pSphere = (kpuBoundingSphere*)&bOther;
			return kpuCollisionDetection::SphereVsSphere(m_vLocation, m_fRadius, pSphere->m_vLocation, pSphere->m_fRadius); 
		}
	case eVT_Box:
		{
			kpuBoundingBox* pBox = (kpuBoundingBox*)&bOther;
			return kpuCollisionDetection::SphereVsBox(m_vLocation, m_fRadius, pBox->GetMin(), pBox->GetMax());
		}
	case eVT_Capsule:
		{
			kpuBoundingCapsule* pCapsule = (kpuBoundingCapsule*)&bOther;
			return kpuCollisionDetection::SphereVsCapsule(m_vLocation, m_fRadius, pCapsule->GetStart(), pCapsule->GetEnd(), pCapsule->GetRadius());
		}
	}

	return collisionData;
	
}

void kpuBoundingSphere::Transform(const kpuMatrix &matrix)
{
	m_vLocation *= matrix;
}
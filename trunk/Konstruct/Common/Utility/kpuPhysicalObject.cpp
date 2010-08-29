#include "StdAfx.h"
#include "kpuPhysicalObject.h"
#include "Common/Graphics/kpgModel.h"
#include "Common/Utility/kpuQuadTree.h"
#include "Common/Graphics/kpgGeometryInstance.h"
#include "Common/Graphics/kpgGeometry.h"
#include "Common/Graphics/kpgVertexBuffer.h"

kpuPhysicalObject::kpuPhysicalObject(void)
{
	m_pModel = 0;	
	Init(kpuv_Zero, kpuv_Zero);

}

kpuPhysicalObject::kpuPhysicalObject(kpuVector vMin, kpuVector vMax, kpgModel* pModel)
{
	m_pModel = pModel;
	Init(vMin, vMax);
}

kpuPhysicalObject::~kpuPhysicalObject(void)
{
	if(m_pModel)
		delete m_pModel;
	/*for(int i = 0; i < m_aCollisionPrimatives.Count(); i++)
	{
		delete m_aCollisionPrimatives[i];
	}*/

}

//kpuBoundingVolume*  kpuPhysicalObject::GetPrimative(int i)
//{
//	kpuBoundingVolume* volume = m_aCollisionPrimatives[i];
//
//	switch(volume->GetType())
//	{
//	case kpuBoundingVolume::eVT_Box:
//		{
//			return kpuBoundingBox(*(kpuBoundingBox*)m_aCollisionPrimatives[i]);
//		}
//	case kpuBoundingVolume::eVT_Sphere:
//		{
//			return kpuBoundingSphere(*(kpuBoundingSphere*)m_aCollisionPrimatives[i]);
//		}
//	case kpuBoundingVolume::eVT_Capsule:
//		{
//			return kpuBoundingCapsule(*(kpuBoundingCapsule*)m_aCollisionPrimatives[i]);
//		}	
//	}
//
//	return volume;
//}

void kpuPhysicalObject::CalculateBoundingVolumes(kpgModel* pCollisionMesh)
{
	kpuArrayList<kpuBoundingVolume*>* aVolumes = pCollisionMesh->GetPrimatives();

	//get smaller primatives
	for(int i = 0; i < aVolumes->Count(); i++ )
    {	
		kpuBoundingVolume* volume = (*aVolumes)[i];		
		m_aCollisionPrimatives.Add(volume);		
    }

	m_bBox = pCollisionMesh->GetBoundingBox();
	m_bSphere = pCollisionMesh->GetSphere();	

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
	float fRadius = (vMax- vMin).Length();

	kpuVector vCenter = (vMax - vMin ) * 0.5 + vMin;

	m_bSphere = kpuBoundingSphere(fRadius / 2, vCenter);
}

void kpuPhysicalObject::Move(kpuVector vVel)
{
	if( vVel != kpuv_Zero )
	{
		m_pCurrentNode->Remove(this);

		float fPercent = m_pCurrentNode->Move(vVel, this);
		vVel *= fPercent;

		SetLocation(GetLocation() + vVel);	

		m_pCurrentNode->ReAdd(this);	

	}
}

kpuBoundingBox kpuPhysicalObject::CalculateBoundingBox(kpuFixedArray<float> &aFloats)
{
	float fXMin, fXMax, fZMin, fZMax, fYMin, fYMax;
	fXMin = fXMax = aFloats[0];
	fYMin = fYMax = aFloats[1];
	fZMin = fZMax = aFloats[2];

	for(int i = 0; i < aFloats.GetNumElements(); i+=3)
	{
		float fNextX = aFloats[i];
		float fNextY = aFloats[i + 1];
		float fNextZ = aFloats[i + 2];

		//Check X min/max
		if ( fNextX < fXMin )
			fXMin = fNextX;

		if ( fNextX > fXMax )
			fXMax = fNextX;

		//Check Y min/max
		if ( fNextY < fYMin )
			fYMin = fNextY;
		
		if ( fNextY > fYMax )
			fYMax = fNextY;

		//Check Z min/max
		if ( fNextZ < fZMin )
			fZMin = fNextZ;

		if ( fNextZ > fZMax)
			fZMax = fNextZ;

	}

	return kpuBoundingBox(kpuVector(fXMin, fYMin, fZMin, 1.0f), kpuVector(fXMax, fYMax, fZMax, 1.0f));
}


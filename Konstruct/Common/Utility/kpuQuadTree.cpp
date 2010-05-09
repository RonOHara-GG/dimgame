#include "StdAfx.h"
#include "kpuQuadTree.h"
#include "kpuPhysicalObject.h"
#include "kpuCollisionData.h"
#include "kpuBoundingCapsule.h"

kpuQuadTree::kpuQuadTree(kpuVector vLoc, float fWidth, float fHeight)
{
	m_bBox = kpuBoundingBox(vLoc, vLoc + kpuVector(fWidth, 0.0f, fHeight, 0.0f));
	m_pNodes = 0;
	m_pParent = 0;
	m_paObjects = new kpuArrayList<kpuPhysicalObject*>();
}

kpuQuadTree::~kpuQuadTree(void)
{
	if( m_pNodes )
		delete m_pNodes;
}

void kpuQuadTree::Divide()
{
	if( !m_pNodes )
	{
		//Divide tree

		m_pNodes = (kpuQuadTree**)malloc(sizeof(kpuQuadTree*) * 4 );

		kpuVector vDim = m_bBox.GetMax() - m_bBox.GetMin();

		//top left
		m_pNodes[0] = new kpuQuadTree(m_bBox.GetMin(), vDim.GetX() * 0.5, vDim.GetZ() * 0.5);
		m_pNodes[0]->m_pParent = this;
		//top right
		m_pNodes[1] = new kpuQuadTree(kpuVector(m_bBox.GetMin().GetX() + vDim.GetX() / 2, 0.0f, m_bBox.GetMin().GetZ(), 0.0f), vDim.GetX() * 0.5, vDim.GetZ() * 0.5);
		m_pNodes[1]->m_pParent = this;
		//bottom right
		m_pNodes[2] = new kpuQuadTree( m_bBox.GetMin() + ( vDim * 0.5 ), vDim.GetX() * 0.5, vDim.GetZ() * 0.5);
		m_pNodes[2]->m_pParent = this;
		//bottom left
		m_pNodes[3] = new kpuQuadTree(kpuVector(m_bBox.GetMin().GetX(), 0.0f, m_bBox.GetMin().GetZ()  + vDim.GetZ() / 2, 0.0f), vDim.GetX() * 0.5, vDim.GetZ() * 0.5);
		m_pNodes[3]->m_pParent = this;
	}

}



bool kpuQuadTree::Add(kpuPhysicalObject* obj)
{
	if( m_bBox.Contains2D(obj->GetBoundingBox()) )
	{
		Divide();

		if( m_pNodes )
		{
			bool bAdded = false;

			//find which child node it is in
			for(int i = 0; i < NUMBER_OF_KIDS; i++)
			{
				if( m_pNodes[i]->Add(obj) )
				{
					bAdded = true;
					break;
				}
			}

			//couldn't find a fit so it will be put in here
			if( !bAdded )
			{
				m_paObjects->Add(obj);
				obj->SetCurrentNode(this);				
			}

			return true;
		}
	}

	return false;
		
}

void kpuQuadTree::Remove(kpuPhysicalObject* obj)
{
	obj->GetCurrentNode()->m_paObjects->Remove(obj);
	//obj->SetCurrentNode(0);
}

void kpuQuadTree::GetCollisions(kpuBoundingCapsule& bCapsule, kpuArrayList<kpuCollisionData> &collidedObjects)
{	
	//Check from the parent node down for for collison
	if( m_pNodes )
	{
		for(int i = 0; i < NUMBER_OF_KIDS; i++)
		{
			if (bCapsule.Intersects( m_pNodes[i]->m_bBox ).m_bCollided )
				m_pNodes[i]->GetCollisions(bCapsule, collidedObjects);
		}
	}

	for( int i = 0; i < m_paObjects->Count(); i++ )
	{
		kpuPhysicalObject* pTest = (*m_paObjects)[i];
		kpuBoundingSphere sphere = pTest->GetSphere();
		sphere.Transform( pTest->GetMatrix() );

		kpuCollisionData data;

		data = bCapsule.Intersects(sphere);

		if( data.m_bCollided )
		{
			data.m_pObject = pTest;
			collidedObjects.Add(data);
		}		
		
	}
}

float kpuQuadTree::CheckMove(kpuPhysicalObject *pObj, float fVelLength)
{
	//Check from the parent node down for for collison
	if( m_pNodes )
	{
		for(int i = 0; i < NUMBER_OF_KIDS; i++)
		{
			float fDist = m_pNodes[i]->CheckMove(pObj, fVelLength);			

			if (fDist < fVelLength )
				return fDist;
		}
	}

	for( int i = 0; i < m_paObjects->Count(); i++ )
	{
		kpuPhysicalObject* pTest = (*m_paObjects)[i];
		//Check collision 			
		//if( kpuCollisionDetection::SphereCollision(pObj->GetSphere(), pTest->GetSphere()) < 0)
		//{
		//	//Check boxes
		//	if ( kpuCollisionDetection::BoxCollision(pObj->GetBoundingBox(), pTest->GetBoundingBox()) )
		//	{
		//		return 0.0f;
		//		//Check the object broken into smaller cubes			
		//		
		//	}
		//}
		
	}

	return fVelLength;
}
float kpuQuadTree::Move(kpuVector& vVel, kpuPhysicalObject* pObj)
{
	kpuArrayList<kpuCollisionData> aCollisions;

	kpuQuadTree* pCurrentNode = this;

	while ( pCurrentNode->m_pParent )
		pCurrentNode = pCurrentNode->m_pParent;

	kpuBoundingCapsule bCapsule(pObj->GetLocation(), pObj->GetLocation() + vVel, pObj->GetSphere().GetRadius() );

	GetCollisions(bCapsule, aCollisions);

	//check collisions in the closest order
	bool bCollisionFound = false;

	while( !bCollisionFound )
	{
		kpuCollisionData* pClosest = &aCollisions[0];
		int iIndex = 0;

		for(int i = 0; i < aCollisions.Count(); i++ )
		{
			kpuCollisionData* pNext = &aCollisions[i];

			if( pClosest->m_fDist > pNext->m_fDist )
			{
				pClosest = pNext;
				iIndex = i;
			}
		}

		aCollisions.RemoveAt(iIndex);		

		//Check the collision primatives of the closest
		kpuBoundingBox bBoxMover = pObj->GetBoundingBox();
		bBoxMover.Transform( pObj->GetMatrix() );

		kpuBoundingBox bBoxStation = pClosest->m_pObject->GetBoundingBox();
		bBoxStation.Transform( pClosest->m_pObject->GetMatrix() );

		if( bBoxMover.Intersects( bBoxStation ).m_bCollided )
		{
			//check smaller boxes
			for(int i = 0; i < pObj->GetPrimativeCount(); i++)
			{
				kpuBoundingVolume bVolume1 = pObj->GetPrimative(i);
				bVolume1.Transform(pObj->GetMatrix());

				for(int j = 0; j < pClosest->m_pObject->GetPrimativeCount(); j++ )
				{
					kpuBoundingVolume bVolume2 = pClosest->m_pObject->GetPrimative(j);
					bVolume2.Transform(pClosest->m_pObject->GetMatrix());

					if(  bVolume1.Intersects(bVolume2).m_bCollided )
					{
						bCollisionFound = true;
						break;
					}
				}
			}
		}
		
	}

	//float fVelLength = vVel.Dot(vVel);

	//float fDist = CheckMove(pObj, fVelLength);	

	//return fDist / fVelLength;

	if (bCollisionFound)
		return 0.0f;
	
	return 1.0f;
}

bool kpuQuadTree::ReAdd(kpuPhysicalObject* pObj)
{
	kpuQuadTree* pCurrent = pObj->GetCurrentNode();

	//Get highest node and add object
	while( pCurrent->m_pParent )
		pCurrent = pCurrent->m_pParent;

	return pCurrent->Add(pObj);
}
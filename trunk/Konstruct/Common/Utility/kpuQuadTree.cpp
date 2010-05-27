#include "StdAfx.h"
#include "kpuQuadTree.h"
#include "kpuPhysicalObject.h"
#include "kpuCollisionData.h"
#include "kpuBoundingCapsule.h"

kpuQuadTree::kpuQuadTree(kpuVector vLoc, float fWidth, float fHeight)
{
	m_bBox = kpuBoundingBox(vLoc, vLoc + kpuVector(fWidth, 0.0f, fHeight, 1.0f));
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
	kpuBoundingBox bBox = obj->GetBoundingBox();
	bBox.Transform(obj->GetMatrix());

	if( m_bBox.Contains2D(bBox) )
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

void kpuQuadTree::GetCollisions(kpuBoundingCapsule& bCapsule, kpuArrayList<kpuCollisionData>* collidedObjects)
{	
	//Check from the parent node down for for collison
	if( m_pNodes )
	{
		for(int i = 0; i < NUMBER_OF_KIDS; i++)
		{
			if (m_pNodes[i]->m_bBox.Contains2D(bCapsule) )
				m_pNodes[i]->GetCollisions(bCapsule, collidedObjects);
		}
	}

	for( int i = 0; i < m_paObjects->Count(); i++ )
	{
		kpuPhysicalObject* pTest = (*m_paObjects)[i];
		kpuBoundingSphere sphere = pTest->GetSphere();
		//sphere.Transform( pTest->GetMatrix() );

		kpuCollisionData data;

		bCapsule.Intersects(&sphere, pTest->GetMatrix(), data);

		if( data.m_bCollided )
		{
			data.m_pObject = pTest;
			collidedObjects->Add(data);
		}		
		
	}
}

bool kpuQuadTree::CheckCollision(kpuBoundingCapsule& capsule, kpuPhysicalObject* pObj)
{
	//Check from the parent node down for for collison
	if( m_pNodes )
	{
		for(int i = 0; i < NUMBER_OF_KIDS; i++)
		{
			if (m_pNodes[i]->m_bBox.Contains2D(capsule) )
			{
				if( m_pNodes[i]->CheckCollision(capsule, pObj) )
					return true;
			}
		}		
	}

	for( int i = 0; i < m_paObjects->Count(); i++ )
	{
		kpuPhysicalObject* pTest = (*m_paObjects)[i];

		if( pTest != pObj )
		{
			kpuBoundingSphere sphere = pTest->GetSphere();
			//sphere.Transform( pTest->GetMatrix() );

			kpuCollisionData data;
			capsule.Intersects(&sphere, pTest->GetMatrix(), data);

			if( data.m_bCollided )
			{
				if( pTest->GetPrimativeCount() < 1 )
					return true;

				data.m_bCollided = false;			

				for(int j = 0; j < pTest->GetPrimativeCount(); j++)
				{
					kpuBoundingVolume* bVolume = pTest->GetPrimative(j);
					capsule.Intersects(bVolume, pTest->GetMatrix(), data);

					if( data.m_bCollided )
					{
						return true;
					}
				}			
			}	
		}
	}

	return false;
	
}

bool kpuQuadTree::CheckCollision(kpuBoundingSphere& sphere, kpuPhysicalObject* pObj)
{
	//Check from the parent node down for for collison
	if( m_pNodes )
	{
		for(int i = 0; i < NUMBER_OF_KIDS; i++)
		{
			if (m_pNodes[i]->m_bBox.Contains2D(sphere) )
			{
				if( m_pNodes[i]->CheckCollision(sphere, pObj) )
					return true;
			}
		}		
	}

	for( int i = 0; i < m_paObjects->Count(); i++ )
	{
		kpuPhysicalObject* pTest = (*m_paObjects)[i];

		if( pTest != pObj )
		{
			kpuBoundingSphere sphereOther = pTest->GetSphere();
			//sphere.Transform( pTest->GetMatrix() );

			kpuCollisionData data;
			sphere.Intersects(&sphereOther, pTest->GetMatrix(), data);

			if( data.m_bCollided )
			{
				if( pTest->GetPrimativeCount() < 1 )
					return true;

				data.m_bCollided = false;			

				for(int j = 0; j < pTest->GetPrimativeCount(); j++)
				{
					kpuBoundingVolume* bVolume = pTest->GetPrimative(j);
					sphere.Intersects(bVolume, pTest->GetMatrix(), data);

					if( data.m_bCollided )
					{
						return true;
					}
				}	
				
			}	
		}
	}

	return false;
	
}

bool kpuQuadTree::CheckCollision(kpuPhysicalObject *pObj)
{
	kpuBoundingBox bBox = pObj->GetBoundingBox();
	bBox.Transform(pObj->GetMatrix());

	if( m_pNodes )
	{
		for(int i = 0; i < NUMBER_OF_KIDS; i++)
		{
			if (m_pNodes[i]->m_bBox.Contains2D(bBox) )
			{
				bool collided = m_pNodes[i]->CheckCollision(pObj);

				if( collided )
					return true;
			}
		}
		
	}

	for( int i = 0; i < m_paObjects->Count(); i++ )
	{
		//Check spheres
		kpuBoundingSphere bSphere = pObj->GetSphere();
		bSphere.Transform(pObj->GetMatrix());
		kpuPhysicalObject* pTest = (*m_paObjects)[i];
		kpuBoundingSphere sphere = pTest->GetSphere();

		kpuCollisionData data;

		bSphere.Intersects(&sphere, pTest->GetMatrix(), data );

		if( data.m_bCollided )
		{
			data.m_bCollided = false;

			//Check boxes
			kpuBoundingBox bOtherBox = pTest->GetBoundingBox();
			bBox.Intersects(&sphere, pTest->GetMatrix(), data );

			if( data.m_bCollided )
			{
				data.m_bCollided = false;
				//Check smaller primatives

				for(int j = 0; j < pObj->GetPrimativeCount(); j++)
				{
					kpuBoundingVolume* bVolume = pObj->GetPrimative(j);

					for(int k = 0; k < pTest->GetPrimativeCount(); k++)
					{
						kpuBoundingVolume* bOther = pTest->GetPrimative(k);

						switch( bVolume->GetType() )
						{
							case kpuBoundingVolume::eVT_Box:
							{
								kpuBoundingBox box = *((kpuBoundingBox*)bVolume);
								box.Transform(pObj->GetMatrix());
								box.Intersects(bOther, pTest->GetMatrix(), data);								
								break;
							}
							case kpuBoundingVolume::eVT_Sphere:
							{
								kpuBoundingSphere sphere = *((kpuBoundingSphere*)bVolume);
								sphere.Transform(pObj->GetMatrix());
								sphere.Intersects(bOther, pTest->GetMatrix(), data);	
								break;
							}
							case kpuBoundingVolume::eVT_Capsule:
							{
								kpuBoundingCapsule capsule = *((kpuBoundingCapsule*)bVolume);
								capsule.Transform(pObj->GetMatrix());
								capsule.Intersects(bOther, pTest->GetMatrix(), data);	
								break;
							}
						}

						if( data.m_bCollided )
						{
							return true;
						}
					}

				}
			}
		}		
		
	}	

	return false;
}

float kpuQuadTree::Move(kpuVector& vVel, kpuPhysicalObject* pObj)
{
	kpuArrayList<kpuCollisionData> aCollisions;

	kpuQuadTree* pCurrentNode = this;

	while ( pCurrentNode->m_pParent )
		pCurrentNode = pCurrentNode->m_pParent;

	kpuBoundingSphere sphere = pObj->GetSphere();
	sphere.Transform(pObj->GetMatrix());

	kpuBoundingCapsule bCapsule(sphere.GetLocation(), sphere.GetLocation() + vVel, pObj->GetSphere().GetRadius() );

	pCurrentNode->GetCollisions(bCapsule, &aCollisions);

	//check collisions in the closest order	
	kpuCollisionData finalData;
	finalData.m_vVelocity = vVel;
	finalData.m_fVelLength = vVel.Length();
	finalData.m_fDist = finalData.m_fVelLength;

	while( !finalData.m_bCollided && aCollisions.Count() > 0 )
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

		//check capsule vs primatives of closest object
		for(int i = 0; i < pClosest->m_pObject->GetPrimativeCount() && !finalData.m_bCollided; i++)
		{
			kpuBoundingVolume* pVolume = pClosest->m_pObject->GetPrimative(i);
			bCapsule.Intersects(pVolume, pClosest->m_pObject->GetMatrix(), finalData);

			if( finalData.m_bCollided )
			{
				finalData.m_bCollided = false;
				finalData.m_fDist = finalData.m_fVelLength;

				//check smaller primatives of moving object against the one found in this collision
				kpuMatrix mTransform = pObj->GetMatrix();
				mTransform.SetD(mTransform.GetD() + vVel);

				for(int j = 0; j < pObj->GetPrimativeCount(); j++)
				{
					kpuBoundingVolume* pVolume2 = pObj->GetPrimative(j);					

					switch( pVolume2->GetType() )
					{
						case kpuBoundingVolume::eVT_Box:
						{
							kpuBoundingBox box = *((kpuBoundingBox*)pVolume2);
							box.Transform(mTransform);
							box.Intersects(pVolume, pClosest->m_pObject->GetMatrix(), finalData);								
							break;
						}
						case kpuBoundingVolume::eVT_Sphere:
						{
							kpuBoundingSphere sphere = *((kpuBoundingSphere*)pVolume2);
							sphere.Transform(mTransform);
							sphere.Intersects(pVolume, pClosest->m_pObject->GetMatrix(), finalData);	
							break;
						}
						case kpuBoundingVolume::eVT_Capsule:
						{
							kpuBoundingCapsule capsule = *((kpuBoundingCapsule*)pVolume2);
							capsule.Transform(mTransform);
							capsule.Intersects(pVolume, pClosest->m_pObject->GetMatrix(), finalData);	
							break;
						}
					}

					if ( finalData.m_bCollided )
					{
						finalData.m_pObject = pClosest->m_pObject;
						break;
					}
				}
			}
		}

		aCollisions.RemoveAt(iIndex);	

		//Check the collision primatives of the closest
		//kpuBoundingBox bBoxMover = pObj->GetBoundingBox();
		//bBoxMover.Transform( pObj->GetMatrix() );

		//kpuBoundingBox bBoxStation = pClosest->m_pObject->GetBoundingBox();
		//bBoxStation.Transform( pClosest->m_pObject->GetMatrix() );

		//if( bBoxMover.Intersects( bBoxStation ).m_bCollided )
		//{
		//	//check smaller boxes
		//	for(int i = 0; i < pObj->GetPrimativeCount(); i++)
		//	{
		//		kpuBoundingVolume* bVolume1 = pObj->GetPrimative(i);
		//		bVolume1->Transform(pObj->GetMatrix());

		//		for(int j = 0; j < pClosest->m_pObject->GetPrimativeCount(); j++ )
		//		{
		//			kpuBoundingVolume* bVolume2 = pClosest->m_pObject->GetPrimative(j);
		//			bVolume2->Transform(pClosest->m_pObject->GetMatrix());

		//			if( bVolume1->Intersects(*bVolume2).m_bCollided )
		//			{
		//				bCollisionFound = true;
		//				break;
		//			}					
		//		}
		//	}
		//}
		
	}

	return  finalData.m_fDist / finalData.m_fVelLength;
}

bool kpuQuadTree::ReAdd(kpuPhysicalObject* pObj)
{
	if( m_pParent )
		m_pParent->ReAdd(pObj);
	else
		return Add(pObj);
}
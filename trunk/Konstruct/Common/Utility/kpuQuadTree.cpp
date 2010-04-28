#include "StdAfx.h"
#include "kpuQuadTree.h"


kpuQuadTree::kpuQuadTree(kpuVector vLoc, float fWidth, float fHeight)
{
	m_bBox = kpuBoundingBox(vLoc.GetX(), vLoc.GetZ(), fWidth, fHeight);
	m_pNodes = 0;
	m_pParent = 0;
	m_paObjects = new kpuArrayList<GameObject*>();
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
	if( m_bBox.Contains(obj->GetBoundingBox()) )
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
	//search till containing node is found
	if( m_bBox.Contains(obj) )
	{
		bool bRemoved = false;

		if( m_pNodes )
		{
			for(int i = 0; i < NUMBER_OF_KIDS; i++)
			{
				if( m_pNodes[i]->Remove(obj) )
				{
					bRemoved = true;
					break;
				}
			}
		}

		if( !bRemoved )
		{
			m_paObjects->Remove(obj);
			obj->SetCurrentNode(0);
		}
	}

}

void kpuQuadTree::ObjectCollide(kpuPhysicalObject* obj, kpuLinkedList &collidedObjects)
{
	//Check from the parent node down for for collison
	kpuQuadTree* pCurrentNode = obj->GetCurrentNode()->m_pParent;

	if( pCurrentNode->m_pNodes )
	{
		for(int i = 0; i < NUMBER_OF_KIDS; i++)
		{
			m_pNodes[i]->ObjectCollide(obj, collideObjects);
		}
	}

	for( int i = 0; i m_paObjects->Count(); i++ )
	{
		kpuPhysicalObject* testObj = (*m_paObjects)[i];

		if( testObj != obj )
		{
			//Check collision 
			if(obj->GetSphere().Intersects(testObj->GetSphere()))
			{
				collidedObjects.Insert(testObj);
			}

		}
	}
}
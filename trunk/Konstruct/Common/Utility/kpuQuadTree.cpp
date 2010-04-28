#include "StdAfx.h"
#include "kpuQuadTree.h"
#include "Client/Dimensions/GameObject.h"

kpuQuadTree::kpuQuadTree(kpuVector vLoc, float fWidth, float fHeight)
{
	m_bBox = kpuBoundingBox(vLoc.GetX(), vLoc.GetZ(), fWidth, fHeight);
	m_pNodes = 0;
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
		//top right
		m_pNodes[1] = new kpuQuadTree(kpuVector(m_bBox.GetMin().GetX() + vDim.GetX() / 2, 0.0f, m_bBox.GetMin().GetZ(), 0.0f), vDim.GetX() * 0.5, vDim.GetZ() * 0.5);
		//bottom right
		m_pNodes[2] = new kpuQuadTree( m_bBox.GetMin() + ( vDim * 0.5 ), vDim.GetX() * 0.5, vDim.GetZ() * 0.5);
		//bottom left
		m_pNodes[3] = new kpuQuadTree(kpuVector(m_bBox.GetMin().GetX(), 0.0f, m_bBox.GetMin().GetZ()  + vDim.GetZ() / 2, 0.0f), vDim.GetX() * 0.5, vDim.GetZ() * 0.5);
	}

}



bool kpuQuadTree::Add(GameObject *obj)
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
				m_paObjects->Add(obj);

			return true;
		}
	}

	return false;
		
}

void kpuQuadTree::Remove(GameObject *obj)
{
	

}

bool kpuQuadTree::ObjectCollide(GameObject *obj)
{
	
	return false;
}
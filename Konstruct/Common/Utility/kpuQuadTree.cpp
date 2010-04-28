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
	else
	{
		//divide subtrees
		for(int i = 0; i < 4; i++)
		{
			m_pNodes[i]->Divide();
		}
	}

}

void kpuQuadTree::GetContainingNodes(GameObject* obj, kpuLinkedList &nodeList)
{
	if( !m_pNodes )
	{
		//See if the object is in this node
		nodeList.Insert(this);
	}
	else
	{
		//find which child node it is in
		for(int i = 0; i < 4; i++)
		{
			if( obj->GetBoundingBox().Intersects(m_pNodes[i]->m_bBox) )
				m_pNodes[i]->GetContainingNodes(obj, nodeList);
		}

	}
}

void kpuQuadTree::Add(GameObject *obj)
{
	kpuLinkedList containingNodes;

	GetContainingNodes(obj, containingNodes);

	kpuLinkedList* finalList = containingNodes.First();

	while( finalList->Next() )
	{
		kpuQuadTree* node = (kpuQuadTree*)finalList->Next();
		node->m_paObjects->Add(obj);

		finalList = finalList->Next();
	}

}

void kpuQuadTree::Remove(GameObject *obj)
{
	kpuLinkedList containingNodes;

	GetContainingNodes(obj, containingNodes);

	kpuLinkedList* finalList = containingNodes.First();

	while( finalList->Next() )
	{
		kpuQuadTree* node = (kpuQuadTree*)finalList->Next();
		node->m_paObjects->Remove(obj);

		finalList = finalList->Next();
	}

}

bool kpuQuadTree::ObjectCollide(GameObject *obj)
{
	kpuLinkedList containingNodes;

	GetContainingNodes(obj, containingNodes);

	kpuLinkedList* finalList = containingNodes.First();

	while( finalList->Next() )
	{
		kpuQuadTree* node = (kpuQuadTree*)finalList->Next();
		
		for(int i = 0; i < node->m_paObjects->Count(); i++)
		{
			GameObject* obj2 = (*node->m_paObjects)[i];

			if( obj->GetBoundingBox().Intersects(obj2->GetBoundingBox()) )
				return true;
		}

		finalList = finalList->Next();
	}

}
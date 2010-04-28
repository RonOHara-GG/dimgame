#pragma once

#include "Common/Utility/kpuVector.h"
#include "Common/Utility/kpuArrayList.h"
#include "Common/Utility/kpuLinkedList.h"
#include "Common/Utility/kpuBoundingBox.h"

class GameObject;

class kpuQuadTree
{
public:
	kpuQuadTree(kpuVector vLoc, float fWidth, float fHeight);
	~kpuQuadTree(void);

	void Divide(); //Divides the tree into 4 smaller nodes
	void Add(GameObject* obj);
	void Remove(GameObject* obj);
	bool ObjectCollide(GameObject* obj);

private:
	void							GetContainingNodes(GameObject* obj, kpuLinkedList &nodeList);

	kpuArrayList<GameObject*>*		m_paObjects;
	kpuQuadTree**					m_pNodes;
	kpuBoundingBox					m_bBox;


};

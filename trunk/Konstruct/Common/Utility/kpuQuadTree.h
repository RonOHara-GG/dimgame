#pragma once

#include "Common/Utility/kpuVector.h"
#include "Common/Utility/kpuArrayList.h"
#include "Common/Utility/kpuBoundingBox.h"
#include "Common/Utility/kpuLinkedList.h"

class kpuPhysicalObject;

class kpuQuadTree
{
public:
	kpuQuadTree(kpuVector vLoc, float fWidth, float fHeight);
	~kpuQuadTree(void);

	void Divide(); //Divides the tree into 4 smaller nodes
	bool Add(kpuPhysicalObject* obj);
	void Remove(kpuPhysicalObject* obj);
	void ObjectCollide(kpuPhysicalObject* obj, kpuLinkedList &collidedObjects);

private:

	kpuArrayList<GameObject*>*		m_paObjects;
	kpuQuadTree*					m_pParent;
	kpuQuadTree**					m_pNodes;
	kpuBoundingBox					m_bBox;


};

#define NUMBER_OF_KIDS 4

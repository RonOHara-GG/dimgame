#pragma once

#include "kpuCollisionData.h"
#include "kpuVector.h"
#include "kpuArrayList.h"
#include "kpuBoundingBox.h"
#include "kpuLinkedList.h"

class kpuPhysicalObject;
class kpuBoundingCapsule;

class kpuQuadTree
{
public:
	kpuQuadTree(kpuVector vLoc, float fWidth, float fHeight);
	~kpuQuadTree(void);

	void Divide(); //Divides the tree into 4 smaller nodes
	bool Add(kpuPhysicalObject* obj);
	bool ReAdd(kpuPhysicalObject* obj);
	void Remove(kpuPhysicalObject* obj);
	
	float Move(kpuVector& vVel, kpuPhysicalObject* pObj);
	bool CheckCollision(kpuPhysicalObject* pObj);
	bool CheckCollision(kpuBoundingCapsule& bCapsule, kpuPhysicalObject* pObj);
	bool CheckCollision(kpuBoundingSphere& bSphere, kpuPhysicalObject* pObj);
	void GetPossibleCollisions(kpuBoundingVolume& bVolume, kpuArrayList<kpuCollisionData>* collidedObjects);

private:
	kpuArrayList<kpuPhysicalObject*>*		m_paObjects;
	kpuQuadTree*							m_pParent;
	kpuQuadTree**							m_pNodes;
	kpuBoundingBox							m_bBox;


};

#define NUMBER_OF_KIDS 4

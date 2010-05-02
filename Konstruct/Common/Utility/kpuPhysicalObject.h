#pragma once
#include "kpuBoundingBox.h"
#include "kpuBoundingSphere.h"
#include "kpuVector.h"

class kpgModel;
class kpuQuadTree;

class kpuPhysicalObject
{
public:
	kpuPhysicalObject(void);
	kpuPhysicalObject(kpuVector vMin, kpuVector vMax);
	virtual ~kpuPhysicalObject(void);

	void				Init(kpuVector vMin, kpuVector vMax);
	kpuBoundingBox		GetBoundingBox()	{ return m_bBox; }
	kpuBoundingSphere	GetSphere()			{ return m_bSphere; }
	kpgModel*			GetCollisionMesh()	{ return m_pCollisionModel; }
	void				SetCollisionMesh(kpgModel* pMesh) { m_pCollisionModel = pMesh; }

	kpuQuadTree*		GetCurrentNode()	{ return m_pCurrentNode; }
	void				SetCurrentNode(kpuQuadTree* node) { m_pCurrentNode = node; }

	virtual void Move(kpuVector& vVel);
	virtual void HandleCollision(kpuPhysicalObject* pCollision, float fDist, kpuVector& vVel);

protected:
	kpuBoundingSphere	m_bSphere;
	kpuBoundingBox		m_bBox;

	kpuQuadTree*		m_pCurrentNode;
	kpgModel*			m_pCollisionModel;
};

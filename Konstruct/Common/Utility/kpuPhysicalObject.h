#pragma once
#include "kpuBoundingBox.h"
#include "kpuBoundingSphere.h"

class kpuQuadTree;

class kpuPhysicalObject
{
public:
	kpuPhysicalObject(void);
	virtual ~kpuPhysicalObject(void);

	kpuBoundingBox		GetBoundingBox()	{ return m_bBox; }
	kpuBoundingSphere	GetSphere()			{ return m_bSphere; }
	kpuQuadTree*		GetCurrentNode()	{ return m_pCurrentNode; }
	void				SetCurrentNode(kpuQuadTree* node) { m_pCurrentNode = node; }

protected:
	kpuBoundingSphere	m_bSphere;
	kpuBoundingBox		m_bBox;

	kpuQuadTree*		m_pCurrentNode;
};

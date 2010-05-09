#pragma once
#include "kpuBoundingVolume.h"
#include "kpuBoundingBox.h"
#include "kpuBoundingSphere.h"
#include "kpuVector.h"
#include "kpuArrayList.h"
#include "kpuMatrix.h"

class kpgModel;
class kpuQuadTree;

class kpuPhysicalObject
{
public:
	kpuPhysicalObject(void);
	kpuPhysicalObject(kpuVector vMin, kpuVector vMax);
	virtual ~kpuPhysicalObject(void);

	virtual kpuVector				GetLocation();
	virtual void					SetLocation(const kpuVector& vNewLoc);
	const kpuMatrix&				GetMatrix();
	void							Init(kpuVector vMin, kpuVector vMax);
	kpuBoundingBox					GetBoundingBox()	{ return m_bBox; }
	kpuBoundingSphere				GetSphere()			{ return m_bSphere; }
	kpuBoundingVolume				GetPrimative(int i) { return m_aCollisionPrimatives[i]; }
	int								GetPrimativeCount()	{ return m_aCollisionPrimatives.Count(); }
	void							SetPrimatives(kpuArrayList<kpuBoundingVolume> &aVolumes);

	kpuQuadTree*					GetCurrentNode()	{ return m_pCurrentNode; }
	void							SetCurrentNode(kpuQuadTree* node) { m_pCurrentNode = node; }

	virtual void					Move(kpuVector& vVel);	

protected:
	kpgModel*							m_pModel;
	kpuBoundingSphere					m_bSphere;
	kpuBoundingBox						m_bBox;
	kpuArrayList<kpuBoundingVolume>		m_aCollisionPrimatives;

	kpuQuadTree*		m_pCurrentNode;
	
};

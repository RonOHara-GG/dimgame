#pragma once
#include "kpuBoundingVolume.h"
#include "kpuBoundingBox.h"
#include "kpuBoundingSphere.h"
#include "kpuVector.h"
#include "kpuArrayList.h"
#include "kpuMatrix.h"
#include "kpuFixedArray.h"

class kpgModel;
class kpuQuadTree;

class kpuPhysicalObject
{
public:
	kpuPhysicalObject(void);
	kpuPhysicalObject(kpuVector vMin, kpuVector vMax, kpgModel* pModel);
	virtual ~kpuPhysicalObject(void);

	virtual kpuVector					GetLocation();
	virtual void						SetLocation(const kpuVector& vNewLoc);
	const kpuMatrix&					GetMatrix();
	kpgModel*							GetModel()	{ return m_pModel; }
	void								Init(kpuVector vMin, kpuVector vMax);
	
	kpuBoundingBox						GetBoundingBox()										{ return m_bBox; }
	kpuBoundingSphere					GetSphere()												{ return m_bSphere; }
	kpuBoundingVolume*					GetPrimative(int i)										{ return m_aCollisionPrimatives[i]; }
	kpuArrayList<kpuBoundingVolume*>*	GetPrimatives()											{ return &m_aCollisionPrimatives; }
	int									GetPrimativeCount()										{ return m_aCollisionPrimatives.Count(); }
	void								CalculateBoundingVolumes(kpgModel* pCollisionMesh);

	kpuQuadTree*						GetCurrentNode()										{ return m_pCurrentNode; }
	void								SetCurrentNode(kpuQuadTree* node)						{ m_pCurrentNode = node; }

	virtual void						Move(kpuVector vVel);	
	virtual void						AreaEffect(kpuVector vCenter, float fRadius, void* pEffect, void* pSource) { }

	void								SetFlag(unsigned int flag) { m_nFlags |= flag; }
	void								UnsetFlag(unsigned int flag) { m_nFlags &= ~flag; }
	bool								HasFlag(unsigned int flag) { return ((flag & m_nFlags) != 0); }
	

protected:
	kpuBoundingBox						CalculateBoundingBox(kpuFixedArray<float> &aFloats);

	kpgModel*							m_pModel;
	kpuBoundingSphere					m_bSphere;
	kpuBoundingBox						m_bBox;
	kpuArrayList<kpuBoundingVolume*>	m_aCollisionPrimatives;

	kpuQuadTree*						m_pCurrentNode;

	u32									m_nFlags;	
};

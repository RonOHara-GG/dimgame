#pragma once
#include "Common/Utility/kpuBoundingVolume.h"
#include "kpuBoundingCapsule.h"

class kpuBoundingBox: public kpuBoundingVolume
{
public:
	kpuBoundingBox(kpuVector vMin = kpuv_OneW ,kpuVector vMax = kpuv_OneW );
	kpuBoundingBox(const kpuBoundingBox& bBox);
	~kpuBoundingBox(void);

	void operator=(const kpuBoundingBox& bBox);

	void Intersects(kpuBoundingVolume* bOther, const kpuMatrix& matrix, kpuCollisionData& data);
	void Transform(const kpuMatrix& matrix);
	

	bool Contains2D(kpuBoundingBox& bBox);
	bool Contains2D(kpuBoundingCapsule& bCapsule);

	kpuVector GetMax()	{ return m_vCenter + m_vOffset; }
	kpuVector GetMin()	{ return m_vCenter - m_vOffset; }
	kpuVector GetCenter()	{ return m_vCenter; }
	kpuVector GetOffset()	{ return m_vOffset; }

	void Move(kpuVector vVel)	{ m_vCenter +=  vVel; }

private:
	kpuVector m_vCenter;
	kpuVector m_vOffset;

};
